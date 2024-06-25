#pragma once

class Profile {
	// information relating to a person

};

// Available Documents
// Access Permit
//  - name
//  - country
//  - passportNumber
//  - purpose
//  - durationOfStay
//  - height
//  - weight
//  - physicalAppearance
//  - expirationDate
// Certificate of Vaccination
//  - name
//  - passportNumber
//  - vaccination1
//  - vaccination2
//  - vaccination3
// Diplomatic Authorization
//  - name
//  - passportNumber
//  - issuingCountry
//  - countryList
// Entry Permit
//  - name
//  - passportNumber
//  - purpose
//  - duration
//  - expirationDate
// Entry Ticket
//  - validDate
// Grant of Asylum
//  - name
//  - country
//  - passportNumber
//  - dateOfBirth
//  - height
//  - weight
//  - expirationDate
// Identity Card
//  - district
//  - name
//  - dateOfBirth
//  - height
//  - weight
// Identitiy Supplement
//  - height
//  - weight
//  - description
//  - expirationDate
// Passport
//  - name
//  - dateOfBirth
//  - sex
//  - issuingCity
//  - expirationDate
//  - passportNumber
//  - passportType -- DEPENDING ON RULE
// Work Pass
//  - name 
//  - field
//  - endDate


// Current Rules


// Booth Information
// Current Date
// Current Weight
// Current Height -- Hard to check
// Current Face -- Hard to check
// Current Gender -- Very hard to check


// Needs to check all rules are followed
// Needs to validate each document given
// Needs to validate all document against each other

// Can implement a crude gender and face check, and if it fails then just compare fields, so it should be safe.
// It needs to have no false positives



// Validation Process:
// 1. Acquire rules and build a rule framework
// 2. Load documents
// 3. Make sure every document is valid on it's on merit
//     - That documents haven't expired
//     - That weight, height, gender, face are correct
//     - That cities are corresponding to the correct passport
// 4. Build a profile, by using documents that show the same fields
//