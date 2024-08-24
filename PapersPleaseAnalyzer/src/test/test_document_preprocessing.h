#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents_v2/doc_type.h"

void test_document_preprocessing(const cv::Mat& image, Documents::V2::DocType docType);
void test_document_preprocessing(const std::string& number, Documents::V2::DocType docType);
void test_access_permit_preprocessing(const std::string& number);
void test_booth_preprocessing(const std::string& number);
void test_certificate_of_vaccination_preprocessing(const std::string& number);
void test_diplomatic_authorization_preprocessing(const std::string& number);
void test_entry_permit_preprocessing(const std::string& number);
void test_entry_ticket_preprocessing(const std::string& number);
void test_grant_of_asylum_preprocessing(const std::string& number);
void test_identity_card_preprocessing(const std::string& number);
void test_identity_supplement_preprocessing(const std::string& number);
void test_passport_preprocessing(const std::string& number);
void test_work_pass_preprocessing(const std::string& number);
