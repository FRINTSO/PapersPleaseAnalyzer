#pragma once

// Idea 1:
/*
✅ 1. Split DocStore by Lifetime
You're already considering this with DocPipelineCache and DocMemoryStore, but take it further:

Idea: Make these pure containers—no logic inside. All logic goes into DocTracker, DocValidator, DocAnalyzer, etc.

Result: The split becomes about ownership, not behavior.
*/
struct DocPipelineCache { FixedHashTable<DocType, DocView> views; ... };
struct DocMemoryStore { FixedHashTable<DocType, DocData> longTermData; ... };
DocTracker tracker(DocPipelineCache&, AnalysisContext&);
DocValidator validator(DocMemoryStore&, AnalysisContext&);

// Idea 2:
/*
Your GameAnalyzer is carrying too much memory ownership directly.

Idea: Create FrameContext and EntrantContext that own the correct parts of the doc memory.

Frame-lifetime things (like GameView, DocView, ScanContext) go into FrameContext.

Entrant-lifetime things go into EntrantContext.
*/
struct FrameContext
{
    const GameView* gameView;
    scannable::ScanContext scanContext;
    DocPipelineCache docCache;
};

struct EntrantContext
{
    DocMemoryStore memoryStore;
    int applicantId;
};
// Then GameAnalyzer::Scan() creates a FrameContext, and updates EntrantContext if needed.

// Idea 3:
/*
✅ 5. AnalysisContext Should Be Split or Decoupled
If AnalysisContext is passed to everything, it’s probably too big.

Idea: Split it into:

GameRulesContext — rulebook, current policies

EntrantMetadata — nationality, etc.

DayState — day index, violations, etc.

Only give components the slice they need.
*/

// Idea 4: Could be useful not having to extract DocData every time, storing the original mat and then subtracting with the new one could be faster
/*
✅ 7. Memoize Per-Document Results
If validation is deterministic and expensive:

Idea: Have a DocAnalysisCache that memoizes analysis by doc hash (e.g., image + data)

Especially helpful if documents repeat or don't change
*/

// Idea 5: Minor thing, but could be nice
/*
✅ 8. Explicit Stage Enums or Logging
To improve observability:

Add something like enum class PipelineStage { Scan, Track, Validate, Analyze } and log transitions.
*/

// Idea 6: This could be nice
/*
✅ 9. Inject View/Context Instead of Holding Them
Rather than m_currentGameView, consider this:

Pass GameView and ScanContext directly into the functions that need them.

Keeps GameAnalyzer stateless across frames.
*/
void GameAnalyzer::Scan(const GameView& view)
{
    ScanContext scanContext = ...;
    FrameContext frame{ &view, scanContext, ... };
    DocTracker tracker(frame.docCache, m_analysisContext);
    ...
}

// Idea 7: Could lead somewhere helpful:
/*
📦 2. DocStore: Shared State or Global Bus?
DocStore is a central hub containing DocView, Doc, DocData, and DocAnalysisStatus. But it’s also used as a return mechanism — “dump stuff in here” — which can feel too loose.

Problem: You get unclear ownership and lifecycle. What does each module expect to find inside DocStore? How do they know what's valid and what's not?

Refactor Direction: If you're aiming for stack-based predictability, consider making the DocStore lifecycle explicit. For example:

Make it require an Init(docViews) step.

Make each module (DocTracker, etc.) explicitly operate on inputs and return outputs that mutate the store — ideally through clearly named functions.
*/
struct DocStore
{
    void Initialize(const DocViewList&);
    void AttachDocument(const DocID&, Doc&& doc);
    const Doc* Get(const DocID&) const;
    void MarkAsInvalid(const DocID&, Reason reason);
    // ...
};

// Idea 8: Good idea
/*
🔍 3. DocValidator: Validator or Analyzer?
This one is subtly overloaded. It’s doing both fact extraction (e.g. expiry date checking, issuing city parsing) and rule validation (e.g. “is this issuing city valid for this country?”).

Problem: It’s easy to conflate facts with judgments.

Refactor Direction: Split validation into two phases:

Extraction Phase (within DocAnalyzer): Extract all dates, cities, names, etc. from DocData.

Validation Phase (in DocValidator): Run rules using Doc and extracted facts.

Then you can make this interaction clean:
*/
// DocPipeline
m_analyzer.ExtractFacts(m_store);  // populate DocData
m_validator.ValidateDocs(m_store); // populate RuleViolations


// Idea 9: Good idea:
/*
2. Introduce a Rule Engine
Idea: Move rule evaluation from DocAnalyzer to a dedicated RuleEngine class.

Why: Rules are central to the game, but their logic is scattered across DocAnalyzer and AnalysisContext. A single engine simplifies rule management and extensions.

How:
RuleEngine takes a RuleBook and evaluates rules against documents and entrant data.

Use a data-driven approach where rules are defined as RuleDescriptor structs (action, subject, target).

Trigger rule checks after validation in DocPipeline.

Example:
class RuleEngine {
    RuleContext& rules_;
public:
    RuleEngine(RuleContext& rules) : rules_(rules) {}
    std::vector<ERule> Evaluate(const Document& doc, const EntrantContext& entrant);
};

Impact: Easier to add new rules, centralizes rule logic, and reduces DocAnalyzer complexity.
*/

// Idea 10:
/*
4. Simplify DocStore State Management
Idea: Replace DocAnalysisStatus with a state machine for document processing.

Why: DocAnalysisStatus has multiple boolean flags (isRequired, wasScanned, etc.), which can lead to inconsistent states.

How:
Define a DocumentState enum: NotScanned, Scanned, Validated, Analyzed, Conflict.

Store state in DocStore as a map of DocType to DocumentState.

Add transitions (e.g., Scanned -> Validated) with clear rules.

Example:
enum class DocumentState { NotScanned, Scanned, Validated, Analyzed, Conflict };

class DocStore {
    std::map<DocType, DocumentState> states_;
public:
    void SetState(DocType type, DocumentState state);
    bool IsRequired(DocType type) const;
};

Impact: Reduces state-related bugs, clarifies document lifecycle, and simplifies debugging.
*/

// Idea 11:
/*
5. Make DocTracker a Document Registry
Idea: Repurpose DocTracker as a lightweight registry for tracking document presence and requirements.

Why: DocTracker does too much (tracking, visibility, requirements), overlapping with DocStore and DocPipeline.

How:
Focus DocTracker on registering scanned documents and required types.

Move visibility tracking to DocStore or a new DocumentVisibility component.

Let DocPipeline handle reporting missing documents.

Example:
class DocTracker {
    DocStore& store_;
    std::set<DocRequirement> required_;
public:
    void RegisterScanned(DocType type);
    void AddRequired(DocRequirement req);
    std::vector<DocType> GetMissingRequired() const;
};

Impact: Slimmer DocTracker, clearer responsibilities, and less overlap with other components.
*/

/*
Mix and Match
These ideas can be adopted individually or combined based on your needs:
Quick Wins: Start with splitting AnalysisContext (#1) and simplifying DocStore state (#4) for immediate clarity.

Medium Effort: Add a RuleEngine (#2) and extract validation logic (#6) to streamline rule handling.

Bigger Lift: Introduce events (#3) or a DocumentFactory (#7) for long-term extensibility.
*/

// Idea 12:
/*
How It Fits Your System
Here’s a lightweight implementation concept for a Rule Engine, integrating with your existing classes (DocStore, AnalysisContext, DocPipeline) and supporting the hierarchical rule structure:
1. Rule Engine Class
Purpose: Evaluates rules based on entrant classification and documents.

Inputs: RuleBook (list of rules), EntrantContext (entrant classification), DocStore (document data).

Outputs: List of rule outcomes (e.g., complied, violated, missing documents).

Example:
class RuleEngine {
public:
    RuleEngine(RuleBook& rules, EntrantContext& entrant, DocStore& store)
        : rules_(rules), entrant_(entrant), store_(store) {}

    struct RuleOutcome {
        ERule rule;
        bool complied;
        std::string_view violationReason; // e.g., "Missing ID card"
    };

    std::vector<RuleOutcome> Evaluate(const std::vector<Document>& documents) {
        std::vector<RuleOutcome> outcomes;
        auto entrantClass = entrant_.GetClassification();

        for (const auto& rule : rules_.GetRules()) {
            auto desc = rule.GetDescriptor();
            // Check if rule applies to entrant
            if (!entrantClass.IsTarget(desc.target)) {
                continue;
            }

            // Check rule compliance
            bool complied = CheckRule(desc, documents);
            outcomes.push_back({rule.GetRule(), complied, GetViolationReason(desc, complied)});
        }
        return outcomes;
    }

private:
    bool CheckRule(const RuleDescriptor& desc, const std::vector<Document>& documents) {
        auto requiredDoc = ERuleSubjectToDocType(desc.subject);
        // Base rule: all entrants need a passport
        if (desc.subject == ERuleSubject::Passport) {
            return store_.Contains(requiredDoc.documentType);
        }
        // Conditional rule: Citizens need ID card
        if (desc.target == ERuleTarget::Citizens && desc.subject == ERuleSubject::IdentityCard) {
            return entrant_.GetClassification().IsCitizen() &&
                   store_.Contains(requiredDoc.documentType);
        }
        // Add more rule checks (e.g., Foreigners, Workers)
        return true;
    }

    std::string_view GetViolationReason(const RuleDescriptor& desc, bool complied) {
        if (!complied) {
            return desc.subject == ERuleSubject::Passport ? "Missing passport" :
                   desc.subject == ERuleSubject::IdentityCard ? "Missing ID card" : "";
        }
        return "";
    }

    RuleBook& rules_;
    EntrantContext& entrant_;
    DocStore& store_;
};
*/

// Idea 13:
/*
2. Simplify DocStore
The DocStore is bloated due to storing multiple document representations, analysis statuses, and required documents. Simplify it by focusing on storage only and moving analysis-related logic elsewhere.
Proposed Changes:
Remove DocAnalysisStatus and m_requiredDocuments from DocStore. These belong in DocTracker (for requirements) and DocAnalyzer (for analysis status).

Use a single FixedHashTable for each document type, with a struct that holds DocView, Doc, and DocData together:
struct Document {
    std::optional<DocView> view;
    std::optional<Doc> doc;
    std::optional<DocData> data;
};

class DocStore {
public:
    void StoreView(documents::DocType type, DocView&& view);
    void StoreDoc(documents::DocType type, Doc&& doc);
    void StoreData(documents::DocType type, DocData&& data);

    const Document& Get(documents::DocType type) const;
    Document& Get(documents::DocType type);

    void Clear(); // Clears all non-rulebook documents

private:
    core::FixedHashTable<documents::DocType, Document, static_cast<size_t>(documents::DocType::Count)> m_documents;
};
*/

// Idea 14: Generic object pool
/*
If objects are not coupled to the pool:

Objects of any type can be pooled. This is the big advantage. By decoupling objects from the pool, you may be able to implement a generic reusable pool class.

The “in use” state must be tracked outside the objects. The simplest way to do this is by creating a separate bit field:

template <class TObject>
class GenericPool
{
private:
  static const int POOL_SIZE = 100;

  TObject pool_[POOL_SIZE];
  bool    inUse_[POOL_SIZE];
};
*/