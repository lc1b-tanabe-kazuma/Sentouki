#pragma once
#include "Player/PlayerStateBase.h"

class NormalState : public PlayerStateBase {
public:
    void Update(Player* player) override;

    void OnCollision(Player* player) override;
};