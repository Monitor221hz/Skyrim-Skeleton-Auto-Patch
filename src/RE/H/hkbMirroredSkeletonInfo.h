#pragma once 

#include "RE/H/hkReferencedObject.h"

namespace RE
{
    class hkbMirroredSkeletonInfo : public hkReferencedObject
    {
        inline static constexpr auto RTTI = RTTI_hkbMirroredSkeletonInfo; 
        inline static constexpr auto VTABLE = VTABLE_hkbMirroredSkeletonInfo; 
        public:
        
        hkVector4 mirrorAxis; // 14
        hkArray<uint16_t> bonePairMap; // 24
        hkArray<uint16_t> partitionPairMap; // 34
    };
    static_assert(sizeof(hkbMirroredSkeletonInfo) == 0x40);
}