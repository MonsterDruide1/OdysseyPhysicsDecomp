#include "Scene/SceneObjFactory.h"

#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/SceneObjHolder.h"
#include "Library/Stage/StageRhythm.h"

#include "Amiibo/AmiiboNpcDirector.h"
#include "Amiibo/HelpAmiiboDirector.h"
#include "Item/CoinCollectHolder.h"
#include "Item/CoinCollectWatcher.h"
#include "Layout/KidsModeLayoutAccessor.h"
#include "MapObj/CapMessageMoonNotifier.h"
#include "MapObj/RhyhtmInfoWatcher.h"
#include "MapObj/RouteGuideDirector.h"
#include "Scene/HintPhotoLayoutHolder.h"

#include "Item/CoinCollectHolder.h"
#include "Item/CoinCollectWatcher.h"
#include "Scene/QuestInfoHolder.h"

class DummySceneObj : public al::ISceneObj {
public:
    DummySceneObj() = default;
};

static al::ISceneObj* sceneObjCreator(s32 id) {
    switch (id) {
    case SceneObjID_AmiiboNpcDirector:
        return new AmiiboNpcDirector();

    case SceneObjID_BgmAnimeSyncDirector:
        return new DummySceneObj();

    case SceneObjID_CapManHeroDemoDirector:
        return new DummySceneObj();

    case SceneObjID_CapMessageDirector:
        return new DummySceneObj();

    case SceneObjID_CapMessageMoonNotifier:
        return new CapMessageMoonNotifier();

    case SceneObjID_CoinCollectHolder:
        return new CoinCollectHolder();

    case SceneObjID_CoinCollectWatcher:
        return new CoinCollectWatcher();

    case SceneObjID_CollectBgmPlayer:
        return new DummySceneObj();

    case SceneObjID_EchoEmitterHolder:
        return new DummySceneObj();

    case SceneObjID_ElectricWireCameraTicketHolder:
        return new DummySceneObj();

    case SceneObjID_FukankunZoomObjHolder:
        return new DummySceneObj();

    case SceneObjID_GrowPlantDirector:
        return new DummySceneObj();

    case SceneObjID_GuidePosInfoHolder:
        return new DummySceneObj();

    case SceneObjID_HintPhotoLayoutHolder:
        return new HintPhotoLayoutHolder();

    case SceneObjID_HtmlViewerRequester:
        return new DummySceneObj();

    case SceneObjID_KidsModeLayoutAccessor:
        return new KidsModeLayoutAccessor();

    case SceneObjID_LoginLotteryDirector:
        return new DummySceneObj();

    case SceneObjID_MoviePlayer:
        return new DummySceneObj();

    case SceneObjID_PaintObjHolder:
        return new DummySceneObj();

    case SceneObjID_PlayerStartInfoHolder:
        return new DummySceneObj();

    case SceneObjID_QuestInfoHolder:
        return new QuestInfoHolder(64);

    case SceneObjID_RandomItemSelector:
        return new DummySceneObj();

    case SceneObjID_ReactionObjectActionIndexHolder:
        return new DummySceneObj();

    case SceneObjID_RhyhtmInfoWatcher:
        return new RhyhtmInfoWatcher("");

    case SceneObjID_RouteGuideDirector:
        return new DummySceneObj();

    case SceneObjID_SceneEventNotifier:
        return new DummySceneObj();

    case SceneObjID_alStageSyncCounter:
        return new al::StageSyncCounter();

    case SceneObjID_TalkNpcParamHolder:
        return new DummySceneObj();

    case SceneObjID_TalkNpcSceneEventSwitcher:
        return new DummySceneObj();

    case SceneObjID_TestStageTimeDirector:
        return new DummySceneObj();

    case SceneObjID_TimeBalloonDirector:
        return new DummySceneObj();

    case SceneObjID_TsukkunTraceHolder:
        return new DummySceneObj();

    case SceneObjID_WipeHolderRequester:
        return new DummySceneObj();

    case SceneObjID_YoshiFruitWatcher:
        return new DummySceneObj();

    case SceneObjID_HelpAmiiboDirector:
        return new HelpAmiiboDirector();

    default:
        return new DummySceneObj();
    }
}

al::SceneObjHolder* SceneObjFactory::createSceneObjHolder() {
    return new al::SceneObjHolder(&sceneObjCreator, SceneObjID_Max);
}
