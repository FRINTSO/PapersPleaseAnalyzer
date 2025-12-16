struct inspector {
    date_t current_date;
    day_rules rules;
    bool rules_loaded = false;

    std::string current_entrant;
    case_file entrant_case;

    std::string last_printed;
};

void inspector_step(inspector& ins, const game_screen& screen, const resources_ctx& ctx) {
    observation obs = observe_frame(screen, ctx);

    // day/entrant resets
    if (obs.date != ins.current_date) { /* reset day */ }
    if (obs.entrant_count != ins.current_entrant) { /* reset entrant */ }

    // rulebook
    if (!ins.rules_loaded && obs.rulebook_parsed_ok) {
        ins.rules = obs.rules;
        ins.rules_loaded = true;
    }

    update_case(ins.entrant_case, obs);

    advice adv = decide(ins.rules, ins.entrant_case);

    print_if_changed(ins, adv);
}