#include "hook.h"

#include "RE/H/hkbBoneWeightArray.h"
#include "RE/H/hkClass.h"
#include "RE/H/hkbVariableBindingSet.h"
#include "patch.h"
namespace SkeletonAutoPatch
{

    void BlenderGeneratorActivateHook::Activate(RE::hkbBlenderGenerator *a_generator, const RE::hkbContext &a_context)
    {
        _Activate(a_generator, a_context); 

        auto* character = a_context.character; 

        if (!character) { return; }

        auto setup = character->setup;
        if (setup)
        {
            auto data = setup->data;
            if (data)
            {
                auto stringData = data->stringData;
                if (stringData)
                {
                    auto characterName = stringData->name.c_str();
                    if (characterName && characterName[0] != '\0' && (strcmpi(characterName, "FirstPerson") == 0))
                    {
                        SKSE::log::info("Skipping Bone Switch Generator Activate for FirstPerson");
                        return;
                    }
                }
            }
        }

        for(auto& child : a_generator->children)
        {
            auto variableBindingSet = child->variableBindingSet;
            if (variableBindingSet)
            {
                bool areWeightsBound = false; 
                auto& bindings = variableBindingSet->bindings;
                for (auto &binding : bindings)
                {
                    if (strcmp(binding.memberPath.c_str(), "boneWeights") == 0)
                    {
                        areWeightsBound = true; 
                        break; 
                    }
                }
                if (areWeightsBound)
                {
                    continue; 
                }
            }
            auto* bone_weights = child->boneWeights; 
            if (!bone_weights) { continue; }
            if (Patcher::PatchBoneWeights(character, bone_weights))
            {
                auto nodeName = a_generator->name.c_str();
                if (nodeName && nodeName[0] != '\0')
                {
                    SKSE::log::info("Patched child of {}", nodeName);
                }
            }
        }
    }
    void BoneSwitchActivateHook::Activate(RE::BSBoneSwitchGenerator *a_generator, const RE::hkbContext &a_context)
    {
        _Activate(a_generator, a_context); 
        auto* character = a_context.character; 
        if (!character) { return; }

        auto setup = character->setup;
        if (setup)
        {
            auto data = setup->data;
            if (data)
            {
                auto stringData = data->stringData;
                if (stringData)
                {
                    auto characterName = stringData->name.c_str();
                    if (characterName && characterName[0] != '\0' && (strcmpi(characterName, "FirstPerson") == 0))
                    {
                        SKSE::log::info("Skipping Bone Switch Generator Activate for FirstPerson");
                        return;
                    }
                }
            }
        }
        for(auto& child : a_generator->ChildrenA)
        {
            auto variableBindingSet = child->variableBindingSet;
            if (variableBindingSet)
            {
                bool areWeightsBound = false; 
                auto& bindings = variableBindingSet->bindings;
                for (auto &binding : bindings)
                {
                    if (strcmp(binding.memberPath.c_str(), "spBoneWeight") == 0)
                    {
                        areWeightsBound = true; 
                        break; 
                    }
                }
                if (areWeightsBound)
                {
                    continue; 
                }
            }
            auto* bone_weights = child->spBoneWeight;
            if (!bone_weights) { continue; }

            if (Patcher::PatchBoneWeights(character, bone_weights))
            {
                auto nodeName = a_generator->name.c_str();
                if (nodeName && nodeName[0] != '\0')
                {
                    SKSE::log::info("Patched child of {}", nodeName);
                }

            }
        }
    }

    void SetupCharacterDataHook::SetupCharacterData(RE::hkbCharacterSetup *a_setup, RE::hkbCharacterData *a_data)
    {
        _SetupCharacterData(a_setup, a_data);
        Patcher::PatchCharacter(a_setup, a_data);
    }

    void CharacterSetupHook::SetupCharacter(RE::hkbCharacterSetup *a_setup, uint64_t a_assetLoader, uint64_t unk03)
    {
        _SetupCharacter(a_setup, a_assetLoader, unk03);
        auto* data = a_setup->data.get();
        if (!data)
        {
            return;
        }
        Patcher::PatchCharacter(a_setup, data);
    }

    void InitHavokHook::InitHavok(RE::Actor *a_actor)
    {
        _InitHavok(a_actor);
        RE::BSAnimationGraphManagerPtr graphManager = nullptr;
        a_actor->GetAnimationGraphManager(graphManager);
        if (!graphManager)
        {
            return;
        }
        auto graph = graphManager->graphs[graphManager->GetRuntimeData().activeGraph];
        if (!graph)
        {
            return;
        }
        auto* character = &graph->characterInstance;
        RE::BSSpinLockGuard locker(graphManager->GetRuntimeData().updateLock); 
        if (Patcher::PatchCharacter(character))
        {
            SKSE::log::info("Patched {}", graph->projectName.c_str());
        }
    }

    int32_t AnimationFileManagerHook::Queue(RE::AnimationFileManagerSingleton* a_animationFileManager, const RE::hkbContext &a_context, RE::hkbClipGenerator *a_clipGenerator, RE::BSSynchronizedClipGenerator *a_syncClipGenerator)
    {
        auto result = _Queue(a_animationFileManager, a_context, a_clipGenerator, a_syncClipGenerator);

        auto bindingIndex = a_clipGenerator->animationBindingIndex;
        if (bindingIndex < 0)
        {
            return result;
        }
        auto *character = a_context.character;
        if (!character)
        {
            return result;
        }
        auto setup = character->setup;
        if (!setup)
        {
            return result;
        }
        auto data = setup->data;
        if (!data)
        {
            return result;
        }
        auto stringData = data->stringData;
        if (!stringData)
        {
            return result;
        }
        auto &animationBundleNames = stringData->animationNames;
        if (animationBundleNames.size() <= bindingIndex)
        {
            return result;
        }
        auto *animationFilename = animationBundleNames[bindingIndex].data();
        SKSE::log::info("Animation {}, Queued animations {}, Loaded animations {} Result {}", animationFilename, a_animationFileManager->queuedAnimations.size(), a_animationFileManager->loadedAnimations.size(), result);

        if (a_clipGenerator->userData != 0xC)
        {
            
            a_animationFileManager->Load(a_context, a_clipGenerator, a_syncClipGenerator);
            SKSE::log::info("Forced load of animation {}", animationFilename);
            return 0;
        }


        return result;
    }

    int32_t AnimationFileManagerHook::Unload(RE::AnimationFileManagerSingleton *a_animationFileManager, const RE::hkbContext &a_context, RE::hkbClipGenerator *a_clipGenerator, RE::BSSynchronizedClipGenerator *a_syncClipGenerator)
    {
        auto result = _Unload(a_animationFileManager, a_context, a_clipGenerator, a_syncClipGenerator);
        auto bindingIndex = a_clipGenerator->animationBindingIndex;
        if (bindingIndex < 0)
        {
            return result;
        }
        auto *character = a_context.character;
        if (!character)
        {
            return result;
        }
        auto setup = character->setup;
        if (!setup)
        {
            return result;
        }
        auto data = setup->data;
        if (!data)
        {
            return result;
        }
        auto stringData = data->stringData;
        if (!stringData)
        {
            return result;
        }
        auto &animationBundleNames = stringData->animationNames;
        if (animationBundleNames.size() <= bindingIndex)
        {
            return result;
        }
        auto *animationFilename = animationBundleNames[bindingIndex].data();
        SKSE::log::info("Unloaded animation {}, Queued animations {}, Loaded animations {} Result {}", animationFilename, a_animationFileManager->queuedAnimations.size(), a_animationFileManager->loadedAnimations.size(), result);
        return result;
    }
}