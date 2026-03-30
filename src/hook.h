#pragma once 
#include "RE/B/BSBoneSwitchGenerator.h"
#include "RE/H/hkbBlenderGenerator.h"
#include "RE/H/hkClass.h"
#include "RE/A/AnimationFileManagerSingleton.h"
namespace SkeletonAutoPatch
{
    class BlenderGeneratorActivateHook
    {
        public: 
        static void Install()
        {
			REL::Relocation<uintptr_t> hkbBlenderGeneratorVtbl { RE::VTABLE_hkbBlenderGenerator[0] };
            _Activate = hkbBlenderGeneratorVtbl.write_vfunc(0x4, Activate); 
			// SKSE::log::info("Hook Installed: Blender Generator Activate");
        }
        private:
        static void Activate(RE::hkbBlenderGenerator* a_generator, const RE::hkbContext& a_context); 
        static inline REL::Relocation<decltype(Activate)> _Activate; 
    };

    class BoneSwitchActivateHook
    {
        public: 
        static void Install()
        {
			REL::Relocation<uintptr_t> BSBoneSwitchGeneratorVtbl { RE::VTABLE_BSBoneSwitchGenerator[0] };
            _Activate = BSBoneSwitchGeneratorVtbl.write_vfunc(0x4, Activate); 
			// SKSE::log::info("Hook Installed: BoneSwitchGenerator Activate");
        }
        private:
        static void Activate(RE::BSBoneSwitchGenerator* a_generator, const RE::hkbContext& a_context); 
        static inline REL::Relocation<decltype(Activate)> _Activate; 
    };

    class SetupCharacterDataHook
    {
        public:
        /*
        Direction	Type	Address	Text
Down	p	BShkbAnimationGraph__sub_140AEDD10+5E5	call    hkbCharacterSetup__sub_140A23ED0
Down	p	sub_140B06E90+C7	call    hkbCharacterSetup__sub_140A23ED0
Down	o	.pdata:000000014353E014	RUNTIME_FUNCTION <rva hkbCharacterSetup__sub_140A23ED0, \
        */
        static void Install()
        {
            REL::Relocation<uintptr_t> target { REL::RelocationID(63031, 0), REL::Relocate(0xC7, 0x0) };
            auto& trampoline = SKSE::GetTrampoline();
            _SetupCharacterData = trampoline.write_call<5>(target.address(), SetupCharacterData);
        }
        private:
        static void SetupCharacterData(RE::hkbCharacterSetup* a_setup,RE::hkbCharacterData* a_data);
        static inline REL::Relocation<decltype(SetupCharacterData)> _SetupCharacterData;
    };

    class CharacterSetupHook
    {
        //	p	sub_140B06E90+10A	call    hkbCharacterSetup__sub_140A50B40 X
        //Up	p	sub_140A4EAA0+42	call    hkbCharacterSetup__sub_140A4F360
        public:
        static void Install()
        {
            REL::Relocation<uintptr_t> target { REL::RelocationID(63031, 0), REL::Relocate(0x10A, 0x0) };
            auto& trampoline = SKSE::GetTrampoline();
            _SetupCharacter = trampoline.write_call<5>(target.address(), SetupCharacter);
        }
        private:
        static void SetupCharacter(RE::hkbCharacterSetup* a_setup, uint64_t a_assetLoader, uint64_t unk03); 
        static inline REL::Relocation<decltype(SetupCharacter)> _SetupCharacter;
    };

    class InitHavokHook
    {
        public:
        static void Install()
        {
            REL::Relocation<uintptr_t> target { RE::VTABLE_Character[0] }; 
            _InitHavok = target.write_vfunc(0x66, InitHavok);
        }
        private:
        static void InitHavok(RE::Actor* a_actor); 
        static inline REL::Relocation<decltype(InitHavok)> _InitHavok;
    };
    
    class AnimationFileManagerHook
    {
        public:
        static void Install()
        {
            REL::Relocation<uintptr_t> vtbl { RE::VTABLE_AnimationFileManagerSingleton[0]}; 
            _Queue = vtbl.write_vfunc(0x1, Queue);
            _Unload = vtbl.write_vfunc(0x3, Unload);
        }

        private:
        static int32_t Queue(RE::AnimationFileManagerSingleton* a_animationFileManager, const RE::hkbContext& a_context, RE::hkbClipGenerator* a_clipGenerator, RE::BSSynchronizedClipGenerator* a_syncClipGenerator);
        static inline REL::Relocation<decltype(Queue)> _Queue;

        static int32_t Unload(RE::AnimationFileManagerSingleton* a_animationFileManager, const RE::hkbContext& a_context, RE::hkbClipGenerator* a_clipGenerator, RE::BSSynchronizedClipGenerator* a_syncClipGenerator);
        static inline REL::Relocation<decltype(Unload)> _Unload;

    };

    static void InstallHooks()
    {
        SKSE::AllocTrampoline(14); 
        InitHavokHook::Install();
        BlenderGeneratorActivateHook::Install();
        BoneSwitchActivateHook::Install();
        // AnimationFileManagerHook::Install();

    }


    
}