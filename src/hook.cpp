#include "hook.h"
#include "RE/H/hkbMirroredSkeletonInfo.h"
#include "RE/H/hkaSkeleton.h"
#include "RE/H/hkbBoneWeightArray.h"
namespace SkeletonAutoPatch
{
    void InitHavokHook::InitHavok(RE::bhkCharacterController *a_this, uint32_t a2, RE::Actor *a_actor)
    {
        _InitHavok(a_this, a2, a_actor);
        RE::BSAnimationGraphManagerPtr graphManager;
        if (!a_actor->GetAnimationGraphManager(graphManager) || !graphManager || graphManager->graphs.empty())
        {
            return;
        }
        RE::BSSpinLockGuard lock(graphManager->GetRuntimeData().updateLock);
        auto *character = &graphManager->graphs[0]->characterInstance;
        if (!character)
        {
            return;
        }

        auto setup = character->setup;
        if (!setup)
        {
            return;
        }

        
        auto *data = setup->data.get();
        if (!data)
        {
            return;
        }

        auto skeleton = setup->animationSkeleton;
        if (!skeleton)
        {
            return;
        }


        auto* mirrored_skeleton_info = stl::adjust_pointer<RE::hkRefPtr<RE::hkbMirroredSkeletonInfo>>(&data->stringData, 0x8)->get(); 
        if (!mirrored_skeleton_info)
        {
            return;
        }

        auto &bone_pair_map = mirrored_skeleton_info->bonePairMap;
        auto num_bones = skeleton->bones.size();

        auto num_bone_pairs = bone_pair_map.size();
        if (num_bones == 0 || num_bone_pairs == 0)
        {
            return;
        }

        auto num_missing_bone_pairs = num_bones - num_bone_pairs;
        auto lim = num_bone_pairs + num_missing_bone_pairs;
        if (num_missing_bone_pairs > 0)
        {
            for (int i = num_bone_pairs; i < lim; i++)
            {
                bone_pair_map.push_back(i);
            }
        }
    }
    RE::hkbBehaviorGraph *LoadBehaviorProjectHook::LoadBehaviorProject(const char *folder, const char *name, RE::hkbCharacter &character, uint64_t project_data_handle, RE::BSFixedString &filename, RE::BSScrapArray<RE::hkbBehaviorGraph *> &hgraphs)
    {
        auto result = _LoadBehaviorProject(folder, name, character, project_data_handle, filename, hgraphs);
        auto setup = character.setup;
        if (!setup)
        {
            return result;
        }
        
        auto *data = setup->data.get();
        if (!data)
        {
            return result;
        }

        auto skeleton = setup->animationSkeleton;
        if (!skeleton)
        {
            return result;
        }


        auto* mirrored_skeleton_info = stl::adjust_pointer<RE::hkRefPtr<RE::hkbMirroredSkeletonInfo>>(&data->stringData, 0x8)->get(); 
        if (!mirrored_skeleton_info)
        {
            return result;
        }

        auto &bone_pair_map = mirrored_skeleton_info->bonePairMap;
        auto num_bones = skeleton->bones.size();

        auto num_bone_pairs = bone_pair_map.size();
        if (num_bones == 0 || num_bone_pairs == 0)
        {
            return result;
        }

        auto num_missing_bone_pairs = num_bones - num_bone_pairs;
        auto lim = num_bone_pairs + num_missing_bone_pairs;
        if (num_missing_bone_pairs > 0)
        {
            for (int i = num_bone_pairs; i < lim; i++)
            {
                bone_pair_map.push_back(i);
            }
        }
        SKSE::log::info("Patched bone pairs for project {}", name);
        return _LoadBehaviorProject(folder, name, character, project_data_handle, filename, hgraphs);
    }
}