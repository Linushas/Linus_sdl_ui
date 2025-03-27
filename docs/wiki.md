# Functions
---
## Panel
### createPanel
```c
Panel createPanel(SDL_Rect rect, SDL_Color bg, SDL_Color border_color);
```
### panel_render
```c
void panel_render(SDL_Renderer *rend, Panel p);
```
### panel_update
```c
int panel_update(SDL_Renderer *rend, Panel p, UI_Event *ui_event, int mouse_x, int mouse_y, bool is_mouse_down);
```
### destroyPanel
```c
int destroyPanel(Panel p);
```
### panel_addComponent
```c
int panel_addComponent(Panel p, int type, void *component, char *key);
```
### panel_getComponent
```c
void *panel_getComponent(Panel p, char *key);
```
### panel_hideComponent
```c
void panel_hideComponent(Panel p, char *key, bool hide);
```
### panel_hide
```c
void panel_hide(Panel p, bool hide)
```
---
## Component: Button
### createButton
```c
Button createButton(SDL_Renderer *rend, char *text, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font);
```
### button_event
```c
int button_event(Button b, int mouse_x, int mouse_y);
```
### button_render
```c
void button_render(SDL_Renderer *rend, Button b);
```
### destroyButton
```c
int destroyButton(Button b);
```
### button_setColorsHovered
```c
int button_setColorsHovered(SDL_Renderer *rend, Button b, SDL_Color bg, SDL_Color fg);
```
---
## Component: Checklist
### createChecklist
```c
Checklist createChecklist(int x, int y, int size, SDL_Color fg, TTF_Font *font);
```
### checklist_addItem
```c
int checklist_addItem(SDL_Renderer *rend, Checklist cb, char *text);
```
### destroyChecklist
```c
int destroyChecklist(Checklist cb);
```
### checklist_event
```c
int checklist_event(Checklist cb, int mouse_x, int mouse_y, bool is_mouse_down);
```
### checklist_render
```c
void checklist_render(SDL_Renderer *rend, Checklist cb);
```
---
## Component: Dropdown Menu
### createDropdownMenu
```c
DropdownMenu createDropdownMenu(SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font);
```
### dropdownMenu_addItem
```c
int dropdownMenu_addItem(SDL_Renderer *rend, DropdownMenu dm, char *text);
```
### destroyDropdownMenu
```c
int destroyDropdownMenu(DropdownMenu dm);
```
### dropdownMenu_event
```c
int dropdownMenu_event(DropdownMenu dm, int mouse_x, int mouse_y, bool is_mouse_down);
```
### dropdownMenu_render
```c
void dropdownMenu_render(SDL_Renderer *rend, DropdownMenu dm);
```
### dropdownMenu_setVisibilityTrue
```c
void dropdownMenu_setVisibilityTrue(DropdownMenu dm);
```
### dropdownMenu_getItemText
```c
void dropdownMenu_getItemText(DropdownMenu dm, int idx, char *text);
```
---
## Component: Text Input Field
### createTextInputField
```c
TextInputField createTextInputField(SDL_Renderer *rend, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font);
```
### destroyTextInputField
```c
int destroyTextInputField(TextInputField tif);
```
### textInputField_update
```c
void textInputField_update(SDL_Renderer *rend, TextInputField tif);
```
### textInputField_render
```c
void textInputField_render(SDL_Renderer *rend, TextInputField tif);
```
### textInputField_moveCursor
```c
void textInputField_moveCursor(TextInputField tif, int direction);
```
### textInputField_getFocus
```c
bool textInputField_getFocus(TextInputField tif);
```
### textInputField_updateFocus
```c
bool textInputField_updateFocus(TextInputField tif, int mouse_x, int mouse_y, bool is_mouse_down);
```
### textInputField_updateBuffer
```c
void textInputField_updateBuffer(TextInputField tif, int input_state, char *input);
```
---