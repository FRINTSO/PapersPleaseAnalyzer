#pragma once
#include "paplease/core/string_buffer.h"
#include "paplease/core/pool.h"

#include <opencv2/opencv.hpp>

namespace paplease {
    namespace core {

        class ResourceManager
        {
        public:
            ResourceManager() = default;

            using MatPool = Pool<cv::Mat, 10>;

            StringBuffer& GetStringBuffer() { return m_stringBuffer; }
            MatPool& GetMatPool() { return m_matPool; }

            void Clear()
            {
                m_stringBuffer.Clear();
                m_matPool.Clear();
            }

        private:
            StringBuffer m_stringBuffer{}; // ~2,216 bytes
            MatPool m_matPool{};      // ~480 bytes
        };

    }
}