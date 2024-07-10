#pragma once

#include <basis/seadTypes.h>
#include <prim/seadBitFlag.h>

class PlayerTrigger {
public:
    enum ECollisionTrigger : u32 {
        // used in PlayerStateHipDrop::isLandTrigger
        ECollisionTrigger_val1 = 1,
        // used in PlayerStateDamageLife::exeDead
        ECollisionTrigger_val6 = 6,
        ECollisionTrigger_val7 = 7,
        ECollisionTrigger_val8 = 8,
        // used in PlayerJudgeWallHitDown
        ECollisionTrigger_val9 = 9,
        // used in PlayerTrigger::isOnYoshiHackEnd
        ECollisionTrigger_val1024 = 1024,
    };

    enum EAttackSensorTrigger : u32 {
        // used in PlayerCounterAfterCapCatch::isCapCatch
        EAttackSensorTrigger_val0 = 0,
        EAttackSensorTrigger_val4 = 4,
    };

    enum EActionTrigger : u32 {
        EActionTrigger_val0 = 0,
        EActionTrigger_val1 = 1,
        EActionTrigger_val2 = 2,
        // used in PlayerStateHipDrop
        EActionTrigger_val3 = 3,
        EActionTrigger_val5 = 5,
        EActionTrigger_val6 = 6,
        EActionTrigger_val8 = 8,
        EActionTrigger_val9 = 9,
        EActionTrigger_val10 = 10,
        EActionTrigger_val11 = 11,
        // used in PlayerStateSquat::appear
        EActionTrigger_val12 = 12,
        EActionTrigger_val14 = 14,
        EActionTrigger_val15 = 15,
        EActionTrigger_val16 = 16,
        EActionTrigger_val17 = 17,
        EActionTrigger_val18 = 18,
        EActionTrigger_val19 = 19,
        EActionTrigger_val21 = 21,
        EActionTrigger_val22 = 22,
        EActionTrigger_val24 = 24,
        EActionTrigger_val25 = 25,
        EActionTrigger_val26 = 26,
        EActionTrigger_val27 = 27,
        EActionTrigger_val28 = 28,
        EActionTrigger_val29 = 29,
        // used in PlayerJudgeWallCatch, PlayerJudgeWallKeep
        EActionTrigger_val30 = 30,
        EActionTrigger_val32 = 32,
        EActionTrigger_val33 = 33,
        EActionTrigger_QuickTurn = 34,
    };

    enum EReceiveSensorTrigger : u32 {};

    enum EPreMovementTrigger : u32 {
        // used in PlayerStateDamageLife::appear
        EPreMovementTrigger_val2 = 2,
        EPreMovementTrigger_val5 = 5,
    };

    enum EDemoEndTrigger : u32 {
        EDemoEndTrigger_val0 = 0,
    };

    enum EMaterialChangeTrigger : u32 {
        EMaterialChangeTrigger_val0 = 0,
    };

    PlayerTrigger();
    void set(ECollisionTrigger flag);
    void set(EAttackSensorTrigger flag);
    void set(EActionTrigger flag);
    void set(EReceiveSensorTrigger flag);
    void set(EPreMovementTrigger flag);
    void set(EDemoEndTrigger flag);
    void set(EMaterialChangeTrigger flag);
    void setRecMaterialTrigger(const char* materialTrigger);
    void clearCollisionTrigger();
    void clearAttackSensorTrigger();
    void clearActionTrigger();
    void clearReceiveSensorTrigger();
    void clearPreMovementTrigger();
    void clearDemoEndTrigger();
    void clearMaterialChangeTrigger();
    bool isOn(ECollisionTrigger flag) const;
    bool isOn(EAttackSensorTrigger flag) const;
    bool isOn(EActionTrigger flag) const;
    bool isOn(EReceiveSensorTrigger flag) const;
    bool isOn(EPreMovementTrigger flag) const;
    bool isOn(EDemoEndTrigger flag) const;
    bool isOn(EMaterialChangeTrigger flag) const;
    bool isOnUpperPunchHit() const;
    bool isOnUpperPunchHitToss() const;
    bool isOnAnyDamage() const;
    bool isOnDamageFire() const;
    bool isOnEndHackWithDamage() const;
    bool isOnNoDamageDown() const;
    bool isOnSpinMoveCapThrow() const;
    bool isOnHipDropCancelThrow() const;
    bool isOnYoshiHackEnd() const;
    bool isOnCollisionExpandCheck() const;
    bool tryGetRecMaterialCode(const char** dest) const;

private:
    sead::BitFlag32 mCollisionTrigger = 0;
    sead::BitFlag32 mAttackSensorTrigger;
    sead::BitFlag64 mActionTrigger = 0;
    sead::BitFlag32 mReceiveSensorTrigger = 0;
    sead::BitFlag32 mPreMovementTrigger = 0;
    sead::BitFlag32 mDemoEndTrigger = 0;
    sead::BitFlag32 mMaterialChangeTrigger = 0;
    const char* mRecMaterialTrigger = nullptr;
};

static_assert(sizeof(PlayerTrigger) == 0x28);
