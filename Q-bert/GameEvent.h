#pragma once
namespace dae {
    enum class GameEvent
    {
        PlayerDied,
        PlayerJumped,
        PlayerPickedUp,
        EnemyDied,
        PlayerFell,
        GameOver,
        roundChanged,
        levelChanged
    };
}