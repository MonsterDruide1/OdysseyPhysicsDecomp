#pragma once

#include "Library/Collision/CollisionPartsKeeper.h"
#include "container/seadPtrArray.h"

namespace game {

class CollisionPartsKeeper : public al::ICollisionPartsKeeper {
public:
    CollisionPartsKeeper();
    ~CollisionPartsKeeper();

    void endInit() override;
    void addCollisionParts(al::CollisionParts*) override;
    void connectToCollisionPartsList(al::CollisionParts*) override;
    void disconnectToCollisionPartsList(al::CollisionParts*) override;
    void resetToCollisionPartsList(al::CollisionParts*) override;
    s32 checkStrikePoint(al::HitInfo*, const al::CollisionCheckInfoBase&) const override;
    s32 checkStrikeSphere(al::SphereHitResultBuffer*, const al::SphereCheckInfo&, bool,
                           const sead::Vector3f&) const override;
    s32 checkStrikeArrow(al::ArrowHitResultBuffer*, const al::ArrowCheckInfo&) const override;
    s32 checkStrikeSphereForPlayer(al::SphereHitResultBuffer*,
                                    const al::SphereCheckInfo&) const override;
    s32 checkStrikeDisk(al::DiskHitResultBuffer*, const al::DiskCheckInfo&) const override;
    void searchWithSphere(const al::SphereCheckInfo&,
                          sead::IDelegate1<al::CollisionParts*>&) const override;
    void movement() override;

private:
    sead::PtrArray<al::CollisionParts> mPartsList;
};

}  // namespace game
