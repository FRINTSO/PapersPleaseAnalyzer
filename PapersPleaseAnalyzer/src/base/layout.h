#pragma once
#include "base/common.h"
#include "base/shape.h"

class INameBox {
public:
	virtual ~INameBox() {}
	virtual Rectangle GetNameBox() = 0;
};

class IDateOfBirthBox {
public:
	virtual ~IDateOfBirthBox() {}
	virtual Rectangle GetDateOfBirthBox() = 0;
};

class IExpirationDateBox {
public:
	virtual ~IExpirationDateBox() {}
	virtual Rectangle GetExpirationDateBox() = 0;
};

class IPassportNumberBox {
public:
	virtual ~IPassportNumberBox() {}
	virtual Rectangle GetPassportNumberBox() = 0;
};

class IPhotoBox {
public:
	virtual ~IPhotoBox() {}
	virtual Rectangle GetPhotoBox() = 0;
};

class IHeightBox {
public:
	virtual ~IHeightBox() {}
	virtual Rectangle GetHeightBox() = 0;
};

class IWeightBox {
public:
	virtual ~IWeightBox() {}
	virtual Rectangle GetWeightBox() = 0;
};

class IPurposeBox {
public:
	virtual ~IPurposeBox() {}
	virtual Rectangle GetPurposeBox() = 0;
};

class IDurationOfStayBox {
public:
	virtual ~IDurationOfStayBox() {}
	virtual Rectangle GetDurationOfStayBox() = 0;
};

#pragma region PassportLayout
class PassportLayout : public INameBox, public IDateOfBirthBox, public IExpirationDateBox, public IPassportNumberBox, public IPhotoBox {
public:
	virtual ~PassportLayout() {
	}
	virtual Rectangle GetSexBox() = 0;
	virtual Rectangle GetIssuingCityBox() = 0;
};

// Size passport numberbox should theoretically be maximum of 126, which exceeds current limit
// 
// Antegria
class AntegriaPassportLayout : public PassportLayout {
public:
	virtual ~AntegriaPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(50), DOWNSCALE(220), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(50), DOWNSCALE(238), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(278), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(50), DOWNSCALE(202), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(50), DOWNSCALE(256), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(298), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(166), DOWNSCALE(176) , DOWNSCALE(80), DOWNSCALE(96) };
	}
};

// Arstotzka
class ArstotzkaPassportLayout : public PassportLayout {
public:
	virtual ~ArstotzkaPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(136), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(136), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(176), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(136), DOWNSCALE(196), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(136), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(296), DOWNSCALE(124), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(80), DOWNSCALE(96) };
	}
};

// Impor
class ImporPassportLayout : public PassportLayout {
public:
	virtual ~ImporPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(140), DOWNSCALE(210), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(140), DOWNSCALE(226), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(172), DOWNSCALE(230), DOWNSCALE(16) };
	}
	
	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(140), DOWNSCALE(194), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(140), DOWNSCALE(242), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(128), DOWNSCALE(292), DOWNSCALE(118), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(18), DOWNSCALE(192), DOWNSCALE(80), DOWNSCALE(96) };
	}
};

// Kolechia
class KolechiaPassportLayout : public PassportLayout {
public:
	virtual ~KolechiaPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(230), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(246), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(214), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(136), DOWNSCALE(262), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(134), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(214), DOWNSCALE(80), DOWNSCALE(96) };
	}
};

// Obristan
class ObristanPassportLayout : public PassportLayout {
public:
	virtual ~ObristanPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(238), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(254), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(222), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(270), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(20), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(168), DOWNSCALE(214), DOWNSCALE(80), DOWNSCALE(96) };
	}
};

// Republia
class RepubliaPassportLayout : public PassportLayout {
public:
	virtual ~RepubliaPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(174), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(196), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(54), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(136), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(170), DOWNSCALE(192), DOWNSCALE(80), DOWNSCALE(96) };
	}
};

// United Federation
class UnitedFederationPassportLayout : public PassportLayout {
public:
	virtual ~UnitedFederationPassportLayout() {}

	Rectangle GetSexBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetIssuingCityBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(138), DOWNSCALE(260), DOWNSCALE(106), DOWNSCALE(16) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(134), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(16), DOWNSCALE(212), DOWNSCALE(80), DOWNSCALE(96) };
	}
};

#pragma endregion

class IdentityCardLayout : public INameBox, public IDateOfBirthBox, public IHeightBox, public IWeightBox, public IPhotoBox {
public:
	virtual ~IdentityCardLayout() {}

	Rectangle GetDistrictBox() {
		return Rectangle{ DOWNSCALE(12), DOWNSCALE(22), DOWNSCALE(232), DOWNSCALE(12) };
	}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(100), DOWNSCALE(40), DOWNSCALE(144), DOWNSCALE(30) };
	}

	Rectangle GetDateOfBirthBox() override {
		return Rectangle{ DOWNSCALE(130), DOWNSCALE(80), DOWNSCALE(114), DOWNSCALE(12) };
	}

	Rectangle GetHeightBox() override {
		return Rectangle{ DOWNSCALE(130), DOWNSCALE(100), DOWNSCALE(114), DOWNSCALE(12) };
	}

	Rectangle GetWeightBox() override {
		return Rectangle{ DOWNSCALE(130), DOWNSCALE(120), DOWNSCALE(114), DOWNSCALE(12) };
	}

	Rectangle GetPhotoBox() override {
		return Rectangle{ DOWNSCALE(12), DOWNSCALE(34), DOWNSCALE(78), DOWNSCALE(96) };
	}
};

class DiplomaticAuthorizationLayout {
public:
	virtual ~DiplomaticAuthorizationLayout() {}

	virtual Rectangle GetNameBox() {
		return Rectangle{ DOWNSCALE(74),DOWNSCALE(182),DOWNSCALE(214),DOWNSCALE(12) };
	}

	virtual Rectangle GetPassportNumberBox() {
		return Rectangle{ DOWNSCALE(98), DOWNSCALE(206), DOWNSCALE(190), DOWNSCALE(12) };
	}

	virtual Rectangle GetIssuingCountryBox() {
		return Rectangle{ DOWNSCALE(68), DOWNSCALE(8), DOWNSCALE(140),DOWNSCALE(12) };
	}

	virtual Rectangle GetCountryListBox() {
		return Rectangle{ DOWNSCALE(42), DOWNSCALE(304),DOWNSCALE(240), DOWNSCALE(56) };
	}
};

class EntryTicketLayout {
public:
	virtual ~EntryTicketLayout() {}

	virtual Rectangle GetValidDateBox() {
		return Rectangle{ DOWNSCALE(176), DOWNSCALE(64), DOWNSCALE(90), DOWNSCALE(16) };
	}
};

class EntryPermitLayout : public INameBox, public IPassportNumberBox, public IPurposeBox, public IDurationOfStayBox, public IExpirationDateBox {
public:
	virtual ~EntryPermitLayout() {}

	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(182), DOWNSCALE(238), DOWNSCALE(12) };
	}

	Rectangle GetPassportNumberBox() override {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(248), DOWNSCALE(238), DOWNSCALE(12) };
	}

	Rectangle GetPurposeBox() override {
		return Rectangle{ DOWNSCALE(102), DOWNSCALE(280), DOWNSCALE(166), DOWNSCALE(12) };
	}

	Rectangle GetDurationOfStayBox() override {
		return Rectangle{ DOWNSCALE(102), DOWNSCALE(310), DOWNSCALE(166), DOWNSCALE(12) };
	}

	Rectangle GetExpirationDateBox() override {
		return Rectangle{ DOWNSCALE(102), DOWNSCALE(340), DOWNSCALE(166), DOWNSCALE(12) };
	}
};

class AccessPermitLayout {
public:
	virtual ~AccessPermitLayout() {}

	virtual Rectangle GetNameBox() {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(108), DOWNSCALE(234), DOWNSCALE(16) };
	}

	virtual Rectangle GetCountryBox() {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(152), DOWNSCALE(112), DOWNSCALE(16) };
	}

	virtual Rectangle GetPassportNumberBox() {
		return Rectangle{ DOWNSCALE(156), DOWNSCALE(152), DOWNSCALE(112), DOWNSCALE(16) };
	}

	virtual Rectangle GetPurposeBox() {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(196), DOWNSCALE(112), DOWNSCALE(16) };
	}

	virtual Rectangle GetDurationOfStayBox() {
		return Rectangle{ DOWNSCALE(156), DOWNSCALE(196), DOWNSCALE(112), DOWNSCALE(16) };
	}

	virtual Rectangle GetHeightBox() {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(240), DOWNSCALE(112), DOWNSCALE(16) };
	}

	virtual Rectangle GetWeightBox() {
		return Rectangle{ DOWNSCALE(156), DOWNSCALE(240), DOWNSCALE(112), DOWNSCALE(16) };
	}

	virtual Rectangle GetPhysicalAppearanceBox() {
		return Rectangle{ DOWNSCALE(30), DOWNSCALE(248), DOWNSCALE(234), DOWNSCALE(16) };
	}

	virtual Rectangle GetExpirationDateBox() {
		return Rectangle{ DOWNSCALE(156), DOWNSCALE(328), DOWNSCALE(108), DOWNSCALE(16) };
	}
};

class WorkPassLayout : public INameBox {
public:
	virtual ~WorkPassLayout() {}
	Rectangle GetNameBox() override {
		return Rectangle{ DOWNSCALE(74), DOWNSCALE(134), DOWNSCALE(204), DOWNSCALE(12) };
	}

	Rectangle GetFieldBox() {
		return Rectangle{ DOWNSCALE(74), DOWNSCALE(164), DOWNSCALE(204), DOWNSCALE(12) };
	}

	Rectangle GetEndDateBox() {
		return Rectangle{ DOWNSCALE(74), DOWNSCALE(194), DOWNSCALE(204), DOWNSCALE(12) };
	}
};

class GrantOfAsylumLayout {
public:
	virtual ~GrantOfAsylumLayout() {}
	
	virtual Rectangle GetNameBox() {
		return Rectangle{ DOWNSCALE(144),DOWNSCALE(108),DOWNSCALE(154),DOWNSCALE(34) };
	}

	virtual Rectangle GetPhotoBox() {
		return Rectangle{ DOWNSCALE(20),DOWNSCALE(100),DOWNSCALE(120),DOWNSCALE(144) };
	}

	virtual Rectangle GetCountryBox() {
		return Rectangle{ DOWNSCALE(182),DOWNSCALE(156),DOWNSCALE(116),DOWNSCALE(16) };
	}

	virtual Rectangle GetPassportNumberBox() {
		return Rectangle{ DOWNSCALE(182),DOWNSCALE(174),DOWNSCALE(116),DOWNSCALE(16) };
	}

	virtual Rectangle GetDateOfBirthBox() {
		return Rectangle{ DOWNSCALE(182),DOWNSCALE(192),DOWNSCALE(116),DOWNSCALE(16) };
	}

	virtual Rectangle GetHeightBox() {
		return Rectangle{ DOWNSCALE(182),DOWNSCALE(210),DOWNSCALE(116),DOWNSCALE(16) };
	}

	virtual Rectangle GetWeightBox() {
		return Rectangle{ DOWNSCALE(182),DOWNSCALE(228),DOWNSCALE(116),DOWNSCALE(16) };
	}

	virtual Rectangle GetFingerPrintsBox() {
		return Rectangle{ DOWNSCALE(22),DOWNSCALE(250),DOWNSCALE(278),DOWNSCALE(66) };
	}

	virtual Rectangle GetExpirationDateBox() {
		return Rectangle{ DOWNSCALE(156),DOWNSCALE(326),DOWNSCALE(100),DOWNSCALE(16) };
	}
};

class IdentitySupplementLayout {
public:
	virtual ~IdentitySupplementLayout() {}

	virtual Rectangle GetHeightBox() {
		return Rectangle{ DOWNSCALE(46), DOWNSCALE(64), DOWNSCALE(114), DOWNSCALE(16) };
	}

	virtual Rectangle GetWeightBox() {
		return Rectangle{ DOWNSCALE(46), DOWNSCALE(86), DOWNSCALE(114), DOWNSCALE(16) };
	}

	virtual Rectangle GetDescriptionBox() {
		return Rectangle{ DOWNSCALE(20), DOWNSCALE(132), DOWNSCALE(142), DOWNSCALE(48) };
	}

	virtual Rectangle GetExpirationDateBox() {
		return Rectangle{ DOWNSCALE(52), DOWNSCALE(278), DOWNSCALE(108), DOWNSCALE(16) };
	}

	virtual Rectangle GetThumbprintBox() {
		return Rectangle{ DOWNSCALE(74), DOWNSCALE(182), DOWNSCALE(88), DOWNSCALE(70) };
	}
};

class CertificateOfVaccinationLayout {
public:
	virtual ~CertificateOfVaccinationLayout() {}

	virtual Rectangle GetNameBox() {
		return Rectangle{ DOWNSCALE(32),DOWNSCALE(102),DOWNSCALE(206),DOWNSCALE(12) };
	}

	virtual Rectangle GetPassportNumberBox() {
		return Rectangle{ DOWNSCALE(54),DOWNSCALE(128),DOWNSCALE(184),DOWNSCALE(12) };
	}

	virtual Rectangle GetVaccination1Box() {
		return Rectangle{ DOWNSCALE(34),DOWNSCALE(190),DOWNSCALE(200),DOWNSCALE(12) };
	}

	virtual Rectangle GetVaccination2Box() {
		return Rectangle{ DOWNSCALE(34),DOWNSCALE(214),DOWNSCALE(200),DOWNSCALE(12) };
	}

	virtual Rectangle GetVaccination3Box() {
		return Rectangle{ DOWNSCALE(34),DOWNSCALE(238),DOWNSCALE(200),DOWNSCALE(12) };
	}
};

class BoothLayout : public IWeightBox {
public:
	virtual ~BoothLayout() {}

	Rectangle GetDateBox() {
		return Rectangle{ DOWNSCALE(8), DOWNSCALE(418), DOWNSCALE(56), DOWNSCALE(12) };
	}

	Rectangle GetCounterBox() {
		return Rectangle{ DOWNSCALE(168), DOWNSCALE(416), DOWNSCALE(22), DOWNSCALE(12) };
	}

	Rectangle GetWeightBox() override {
		return Rectangle{ DOWNSCALE(302), DOWNSCALE(390), DOWNSCALE(44), DOWNSCALE(12) };
	}
};