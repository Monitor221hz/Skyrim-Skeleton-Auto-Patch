namespace SkeletonAutoPatch
{
    class InitHavokHook
    {
        public:
            static void Install()
            {
                REL::Relocation<uintptr_t> baseHook{REL::RelocationID(36192, 37171), REL::Relocate(0x34E, 0x3B7)};
                auto &trampoline = SKSE::GetTrampoline();
                _InitHavok = trampoline.write_call<5>(baseHook.address(), InitHavok);
            }

        private:
            static void InitHavok(RE::bhkCharacterController *a_this, uint32_t a2, RE::Actor *a_actor);
            static inline REL::Relocation<decltype(InitHavok)> _InitHavok;
    };
    class LoadBehaviorProjectHook
    {
        public:
        static void Install()
        {
            REL::Relocation<std::uintptr_t> target{ REL::RelocationID(62640, 63585), REL::Relocate(0x108, 0x111) };
            auto &trampoline = SKSE::GetTrampoline();
            _LoadBehaviorProject = trampoline.write_call<5>(target.address(), LoadBehaviorProject);
        }
        private:
        static RE::hkbBehaviorGraph* LoadBehaviorProject(const char* folder, const char* name, RE::hkbCharacter& hchar, uint64_t project_data_handle, RE::BSFixedString& filename, RE::BSScrapArray<RE::hkbBehaviorGraph*>& hgraphs);
        static inline REL::Relocation<decltype(LoadBehaviorProject)> _LoadBehaviorProject;
    };

    static void InstallHooks()
    {
        SKSE::AllocTrampoline(14);
        // LoadBehaviorProjectHook::Install();
        // to-do: hook blend and bone switch generators
        InitHavokHook::Install();
    }

    
}