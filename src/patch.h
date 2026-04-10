#pragma once 
#include <unordered_map>
#include "RE/H/hkbBoneWeightArray.h"
#include "RE/H/hkaSkeleton.h"
#include "RE/H/hkbMirroredSkeletonInfo.h"
namespace SkeletonAutoPatch
{
    class Patcher
    {
        public:
            static bool PatchCharacter(RE::hkbCharacterSetup* a_setup, RE::hkbCharacterData* a_data);
            static bool PatchCharacter(RE::hkbCharacter* a_character); 
            static bool PatchBoneWeights(RE::hkbCharacter* a_character, RE::hkbBoneWeightArray* a_array);

        private:
            static bool PatchBoneWeights(const RE::hkaSkeleton *a_skeleton, RE::hkbMirroredSkeletonInfo *a_mirroredSkeletonInfo, RE::hkbBoneWeightArray *a_boneWeightArray, bool majorityRule = true, float overrideWeight = 0.f);

            static float GetDefaultWeight(RE::hkArray<float> &blend_weights)
            {
                size_t num_zeroes = 0;
                for (auto blend_weight : blend_weights)
                {
                    num_zeroes += blend_weight > 0 ? 0 : 1;
                }
                return num_zeroes > blend_weights.size() / 2 ? 0.f : 1.f; // majority rule
            }

            static inline std::unordered_map<size_t, float> humanoidOverrides{
                { 23, 1.f }, // UpperBodyNoRightArm
                { 22, 0.f }, // ArmsAndHead
                { 19, 0.f }, // Arms
                { 6, 1.f } // LeftArm
            };

    };
}