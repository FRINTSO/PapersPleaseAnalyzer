#pragma once
inline constexpr doc_appearance g_appearances[] = {
    // access_permit
    {{{215, 233, 210}, {176, 168, 173}}, 2, 294, 392},
    // certificate_of_vaccination
    {{{234, 223, 128}, {177, 151, 97}}, 2, 270, 312},
    // diplomatic_authorization
    {{{238, 238, 251}, {190, 202, 209}, {154, 179, 168}}, 3, 300, 400},
    // entry_permit
    {{{224, 233, 199}, {202, 201, 175}}, 2, 300, 402},
    // entry_ticket
    {{{224, 233, 199}, {180, 169, 151}}, 2, 280, 102},
    // grant_of_asylum
    {{{253, 222, 223}, {180, 157, 175}}, 2, 320, 374},
    // identity_card
    {{{217, 189, 247}, {178, 156, 204}}, 2, 252, 142},
    // identity_supplement
    {{{232, 218, 199}, {181, 167, 166}}, 2, 180, 300},
    // work_pass
    {{{233, 199, 211}, {204, 175, 192}}, 2, 294, 270},
    // rule_book
    {{{87, 72, 72}}, 1, 480, 320},
    // bulletin
    {{{240, 240, 240}}, 1, 298, 398},
    // transcript
    {{{210, 237, 236}}, 1, 300, 400},
    // passport_antegria
    {{{49, 77, 33}}, 1, 260, 324},
    // passport_arstotzka
    {{{59, 72, 59}}, 1, 260, 324},
    // passport_impor
    {{{102, 31, 9}}, 1, 260, 324},
    // passport_kolechia
    {{{85, 37, 63}}, 1, 260, 324},
    // passport_obristan
    {{{138, 12, 12}}, 1, 260, 324},
    // passport_republia
    {{{76, 42, 27}}, 1, 260, 324},
    // passport_united_federation
    {{{35, 30, 85}}, 1, 260, 324},
};

constexpr doc_appearance get_appearance(doc_variant v) {
    return g_appearances[static_cast<size_t>(v)];
}
