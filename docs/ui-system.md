# UI System

The UI is built from separate screens and panels instead of one large UI file.

## Main Classes

- `Engine::UIElement` is the base class for drawable/updateable UI objects.
- `Engine::UIManager` owns UI elements and updates/draws visible elements in order.
- `GameUIComponent` connects gameplay state to the game UI.
- `HUD` draws health, armor, level, and enemy objective.
- `InventoryPanel` draws paged inventory slots.
- `EquipmentPanel` accepts equipment items.
- `HotbarPanel` stores usable consumables for number-key shortcuts.
- `PopupMessage` shows short feedback messages.
- `GameScreenOverlay` shows main menu, pause, game over, and level-cleared screens.

## State Handling

`GameUIComponent` handles modal states first:

1. game over;
2. level complete;
3. main menu;
4. pause;
5. inventory and hotbar input.

This order prevents gameplay input from leaking through menu screens. When inventory,
pause, game over, or level-complete screens are active, `GameWorld::SetPaused(true)` blocks
normal gameplay updates. The UI object is marked as pause-ignored, so it still receives
input and can close menus or restart the scene.

## Event-Driven HUD

The HUD does not poll player health every frame. `StatsComponent` exposes
`AddStatsChangedListener`, and `GameUIComponent` subscribes when it receives the player.

The flow is:

1. player receives damage through `StatsComponent::TakeDamage`;
2. `StatsComponent` changes health or armor;
3. `StatsComponent` notifies its listeners;
4. `GameUIComponent::UpdateHUDStats` updates the HUD values.

`UpdateHUD()` still runs every frame for objective text, because enemy count and level
objective are derived from the current level state.

## Run State

The engine exposes `SaveSystem`, a small typed key-value store for data that must survive
scene restarts. Roguelike saves one `PlayerRunSnapshot` before entering the next generated
level and restores it when the new player object is created. The snapshot contains inventory
items, hotbar slots, equipment slots, health, armor, and attack power.

The snapshot is cleared on game-over restart, so a new run starts cleanly.

## Inventory Flow

Inventory data lives in `InventoryComponent` as `std::vector<ItemStack>`. The UI receives
that list through `InventoryPanel::SetItems` and renders slots with loops, so the drawing
logic is not tied to a fixed item count.

Inventory supports:

- open/close with `I`;
- page switching;
- item selection;
- equipment placement;
- hotbar placement for consumables;
- right-click drag cancel;
- popup feedback for invalid actions.

## Animation

`UIAnimation` owns the shared fade state for UI elements. Panels reuse that alpha value
for extra motion where it helps readability:

- screen overlays fade and slide into place when menu, pause, victory, or defeat screens open;
- overlay subtitles use a soft breathing alpha pulse for "Press ..." prompts;
- the start screen cycles six campfire sprites and draws a separate additive fire-light texture
  with irregular flicker and a subtle mouse-based parallax offset. Campfire placement is configured
  in `GameConfig` so the sprite can be nudged without changing overlay logic;
- the HUD draws a quest list texture in the top-right corner with the current level and remaining
  enemies, keeping objective text grouped in one readable panel;
- `DirectionalSpriteComponent` switches player and enemy textures from movement velocity, so
  movement code and sprite-selection code stay separate;
- the death screen cross-fades two background textures with a slightly uneven light fade;
- popup messages fade in with a small upward motion and float out near the end of their lifetime;
- hotbar slots pulse when a number key is pressed.

## Defense Notes

The important design point is separation:

- `InventoryComponent` stores data;
- UI panels draw and interact with data;
- `GameUIComponent` coordinates state transitions;
- `GameWorld` pauses gameplay while modal UI is active.
