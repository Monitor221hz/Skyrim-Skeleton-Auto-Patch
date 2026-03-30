#include "RE/A/AnimationFileManagerSingleton.h"

namespace RE
{
    bool AnimationFileManagerSingleton::Load(const hkbContext &a_hkbContext, hkbClipGenerator *a_clipGenerator, BSSynchronizedClipGenerator *a_synchronizedClipGenerator)
    {
        return REL::RelocateVirtual<decltype(&AnimationFileManagerSingleton::Load)>(0x2, 0x2, this, a_hkbContext, a_clipGenerator, a_synchronizedClipGenerator);
    }
}