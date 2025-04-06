#include "Library/Effect/EffectSystem.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneUtil.h"

void al::initActorInitInfo(al::ActorInitInfo* a1, const al::Scene* a2, const al::PlacementInfo* a3,
                           const al::LayoutInitInfo* a4, const al::ActorFactory* a5,
                           al::SceneMsgCtrl* a6, al::GameDataHolderBase* a7) {
    al::LiveActorKit* mLiveActorKit;
    al::EffectSystem* mEffectSystem;
    const al::GamePadSystem* mGamePadSystem;
    al::PlayerHolder* mPlayerHolder;
    al::HitSensorDirector* a17;
    al::EffectSystemInfo* a13;
    al::CollisionDirector* a11;
    al::ItemDirectorBase* _90;
    al::NatureDirector* a19;
    al::PadRumbleDirector* a21;
    al::SceneObjHolder* a23;
    al::GravityHolder* a16;
    al::ExecuteDirector* a14;
    al::DemoDirector* a12;
    al::ClippingDirector* a10;
    al::CameraDirector* a9;
    al::LiveActorGroup* mLiveActorGroupAllActors;
    al::ActorResourceHolder* mActorResourceHolder;
    al::AreaObjDirector* mAreaObjDirector;
    al::AudioDirector* mAudioDirector;

    mLiveActorKit = a2->mLiveActorKit;
    mAudioDirector = a2->mAudioDirector;
    mActorResourceHolder = mLiveActorKit->mActorResourceHolder;
    mAreaObjDirector = mLiveActorKit->mAreaObjDirector;
    a9 = mLiveActorKit->mCameraDirector;
    a10 = mLiveActorKit->mClippingDirector;
    mLiveActorGroupAllActors = mLiveActorKit->mLiveActorGroupAllActors;
    a12 = mLiveActorKit->mDemoDirector;
    mEffectSystem = mLiveActorKit->mEffectSystem;
    a14 = mLiveActorKit->mExecuteDirector;
    a16 = mLiveActorKit->mGravityHolder;
    mGamePadSystem = mLiveActorKit->mGamePadSystem;
    mPlayerHolder = mLiveActorKit->mPlayerHolder;
    a17 = mLiveActorKit->mHitSensorDirector;
    if (mEffectSystem) {
        //a13 = &mEffectSystem->mInfo;
        WARN_UNIMPL;
        a13 = nullptr;
    }
    else
        a13 = 0LL;

    a11 = mLiveActorKit->mCollisionDirector;
    _90 = (al::ItemDirectorBase*)mLiveActorKit->mItemDirector;
    a21 = mLiveActorKit->mPadRumbleDirector;
    a19 = mLiveActorKit->mNatureDirector;
    a23 = a2->getSceneObjHolder();
    a1->initNew(
        a3, a4, mLiveActorGroupAllActors, a5, mActorResourceHolder, mAreaObjDirector,
        mAudioDirector, a9, a10, a11, a12, a13, a14, a7, a16, a17, _90, a19, mGamePadSystem, a21,
        mPlayerHolder, a23, a6, a2->mSceneStopCtrl, a2->mScreenCoverCtrl,
        mLiveActorKit->mScreenPointDirector, (al::ShadowDirector*)mLiveActorKit->mShadowDirector,
        mLiveActorKit->mStageSwitchDirector, mLiveActorKit->mModelGroup,
        mLiveActorKit->mGraphicsSystemInfo,
        (al::ModelDrawBufferCounter*)mLiveActorKit->mDrawBufferDirector,
        mLiveActorKit->mDynamicDrawActorGroup);
}
