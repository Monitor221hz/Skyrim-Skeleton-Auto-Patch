#include "patch.h"
#include "RE/H/hkbVariableValueSet.h"
#include "RE/H/hkClass.h"
namespace SkeletonAutoPatch
{
    bool Patcher::PatchBoneWeights(RE::hkbCharacter *a_character, RE::hkbBoneWeightArray *a_array)
    {
        if (!a_character)
        {
            return false;
        }

        auto setup = a_character->setup;
        if (!setup)
        {
            return false;
        }

        auto *data = setup->data.get();

        auto skeleton = setup->animationSkeleton;
        if (!skeleton)
        {
            return false;
        }

        auto* mirroredSkeletonInfo = stl::adjust_pointer<RE::hkRefPtr<RE::hkbMirroredSkeletonInfo>>(&data->stringData, 0x8)->get(); 
        if (!mirroredSkeletonInfo)
        {
            return false;
        }
        return PatchBoneWeights(skeleton.get(), mirroredSkeletonInfo, a_array);
    }
    bool Patcher::PatchCharacter(RE::hkbCharacterSetup *a_setup, RE::hkbCharacterData *a_data)
    {
        auto data = a_setup->data;
        if (data)
        {
            auto stringData = data->stringData;
            if (stringData)
            {
                auto characterName = stringData->name.c_str();
                if (characterName && characterName[0] != '\0' && (strcmp(characterName, "FirstPerson") == 0))
                {
                    SKSE::log::info("Skipping Patch Character for FirstPerson");
                    return false;
                }
            }
        }

        auto skeleton = a_setup->animationSkeleton;
        if (!skeleton)
        {
            return false;
        }

        auto* mirroredSkeletonInfo = stl::adjust_pointer<RE::hkRefPtr<RE::hkbMirroredSkeletonInfo>>(&a_data->stringData, 0x8)->get(); 
        if (!mirroredSkeletonInfo)
        {
            return false;
        }

        auto &bonePairMap = mirroredSkeletonInfo->bonePairMap;
        auto boneCount = skeleton->bones.size();

        auto bonePairCount = bonePairMap.size();
        if (boneCount == 0 || bonePairCount == 0 || boneCount == bonePairCount)
        {
            return false;
        }

        auto missingBonePairCount = boneCount - bonePairCount;
        auto limit = bonePairCount + missingBonePairCount;
        if (missingBonePairCount <= 0)
        {
            return false;
        }
        for (int i = bonePairCount; i < limit; i++)
        {
            bonePairMap.push_back(i);
        }

        auto variableValueSet = stl::adjust_pointer<RE::hkRefPtr<RE::hkbVariableValueSet>>(&a_data->characterControllerSetup, 0x70)->get();
        if (!variableValueSet)
        {
            return false;
        }

        auto& variants = variableValueSet->variantVariableValues;
        for(auto& variant : variants)
        {
            /* I have yet to see a vanilla behavior project that doesn't use bone weight arrays exclusively in variants. If this ever changes, 
               this needs to be changed to include a class check but class getter seems nonexistent for bone arrays. */
            auto* boneWeightArray = reinterpret_cast<RE::hkRefPtr<RE::hkbBoneWeightArray>*>(&variant)->get();
            if (!boneWeightArray)
            {
                continue;
            }
            PatchBoneWeights(skeleton.get(), mirroredSkeletonInfo, boneWeightArray);
        }
        return true;
    }
    bool Patcher::PatchCharacter(RE::hkbCharacter *a_character)
    {
        auto setup = a_character->setup;
        if (!setup)
        {
            return false;
        }
        
        auto data = setup->data;
        if (!data)
        {
            return false;
        }
        return PatchCharacter(setup.get(), data.get());
    }
    bool Patcher::PatchBoneWeights(const RE::hkaSkeleton *a_skeleton, RE::hkbMirroredSkeletonInfo *a_mirroredSkeletonInfo, RE::hkbBoneWeightArray *a_boneWeightArray, bool majorityRule)
    {
        if (!a_skeleton)
        {
            return false; 
        }
        auto& boneWeights = a_boneWeightArray->weights;
        if (boneWeights.size() == a_skeleton->bones.size()
            || boneWeights.size() <= 1)
        {
            return false; 
        }


        float defaultWeight = majorityRule ?  GetDefaultWeight(boneWeights) : 0.f;
        int boneWeightCount = boneWeights.size();
        int missingWeightCount = a_skeleton->bones.size() - boneWeightCount;
        if (missingWeightCount <= 1)
        {
            return false; // skip bow zoom weights
        }

        int limit = missingWeightCount + boneWeightCount;
        boneWeights.resize(limit);
        if (!a_mirroredSkeletonInfo)
        {
            for (int i = boneWeightCount; i < limit; i++)
            {
                boneWeights[i] = defaultWeight;
            }
            return true;
        }
        int bonePairCount = a_mirroredSkeletonInfo->bonePairMap.size();
        for(int i = boneWeightCount; i < limit; i++)
        {
            if (i >= bonePairCount)
            {
                boneWeights[i] = defaultWeight;
                continue;
            }
            auto mirroredIndex = a_mirroredSkeletonInfo->bonePairMap[i];
            if (mirroredIndex >= boneWeights.size())
            {
                boneWeights[i] = defaultWeight;
                continue;
            }
            boneWeights[i] = boneWeights[mirroredIndex];
        }
        return true; 
    }
}