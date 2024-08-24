#pragma once
#include <memory>
#include <utility>

#include <opencv2/opencv.hpp>

#include "base/shape.h"

enum class DocumentType {
	AccessPermit,
	Booth,
	CertificateOfVaccination,
	DiplomaticAuthorization,
	EntryPermit,
	EntryTicket,
	GrantOfAsylum,
	IdentityCard,
	IdentitySupplement,
	Passport,
	WorkPass,
};

template<typename Layout>
class Document {
public:
	cv::Mat mat;
	std::unique_ptr<Layout> layoutProvider;

	Document(cv::Mat mat) : mat(mat), layoutProvider(std::make_unique<Layout>()) {}
	Document(cv::Mat mat, std::unique_ptr<Layout> provider) : mat(mat), layoutProvider(std::move(provider)) {}

	//Document(const Document&) = delete;
	//Document& operator=(const Document&) = delete;
};

