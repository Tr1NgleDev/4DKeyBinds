// #define DEBUG_CONSOLE // Uncomment this if you want a debug console

// Mod Name. Make sure it matches the mod folder's name. Also don't forget to change the output DLL name in Project Properties->General->Target Name
#define MOD_NAME "4DKeyBinds"
#define MOD_VER "1.0"

#include <Windows.h>
#include <cstdio>
#include <4dm.h>

#include "GLFWKeys.h"
using namespace glfw;
using namespace fdm;

#include <fstream>

template<typename T>
bool containsVal(const std::vector<T>& vec, const T& value)
{
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

std::vector<std::string> splitByDelim(const std::string& str, char delim)
{
	std::vector<std::string> tokens;
	size_t pos = 0;
	size_t len = str.length();
	tokens.reserve(len / 2); // allocate memory for expected number of tokens

	while (pos < len)
	{
		size_t end = str.find_first_of(delim, pos);
		if (end == std::string::npos)
		{
			tokens.emplace_back(str.substr(pos));
			break;
		}
		tokens.emplace_back(str.substr(pos, end - pos));
		pos = end + 1;
	}

	return tokens;
}

std::pair<std::string, std::string> splitBindName(const std::string& str)
{
	size_t colonPos = str.find(':');
	if (colonPos != std::string::npos) 
	{
		std::string first = str.substr(0, colonPos);
		std::string second = str.substr(colonPos + 1);
		return std::make_pair(first, second);
	}

	return std::make_pair("", "");
}

int getWidthOfElement(gui::Element* e)
{
	if (gui::Button* button = dynamic_cast<gui::Button*>(e))
		return button->width;
	if (gui::Text* text = dynamic_cast<gui::Text*>(e))
		return text->text.size() * text->size * 8;
	return 20;
}

enum KeyBindsScope
{
	GLOBAL,
	PLAYER,
	STATEGAME,
	STATETITLESCREEN,
	TEXTINPUT
};

std::map<KeyBindsScope, std::vector<std::string>> namesOrder =
{
	{
		KeyBindsScope::GLOBAL,
		{
		}
	},
	{
		KeyBindsScope::PLAYER,
		{
			"4D Miner:Jump",
			"4D Miner:Left",
			"4D Miner:Right",
			"4D Miner:Back",
			"4D Miner:Forward",
			"4D Miner:Strafe W+",
			"4D Miner:Strafe W-",
			"4D Miner:Crouch",
			"4D Miner:Sprint",
			"4D Miner:Drop",
			"4D Miner:Inventory",
			"4D Miner:Look 4D",
			"4D Miner:Slot 1",
			"4D Miner:Slot 2",
			"4D Miner:Slot 3",
			"4D Miner:Slot 4",
			"4D Miner:Slot 5",
			"4D Miner:Slot 6",
			"4D Miner:Slot 7",
			"4D Miner:Slot 8",
			"4D Miner:Chunks Reload"
		}
	},
	{
		KeyBindsScope::STATEGAME,
		{
		}
	},
	{
		KeyBindsScope::STATETITLESCREEN,
		{
		}
	},
	{
		KeyBindsScope::TEXTINPUT,
		{
		}
	}
};

std::unordered_map<KeyBindsScope, std::unordered_map<std::string, Keys>> keyBinds =
{
	{
		KeyBindsScope::GLOBAL,
		{
			
		}
	},
	{
		KeyBindsScope::PLAYER,
		{
			{ "4D Miner:Jump", Keys::Space },
			{ "4D Miner:Left", Keys::A },
			{ "4D Miner:Right", Keys::D },
			{ "4D Miner:Back", Keys::S },
			{ "4D Miner:Forward", Keys::W },
			{ "4D Miner:Strafe W+", Keys::E },
			{ "4D Miner:Strafe W-", Keys::Q },
			{ "4D Miner:Crouch", Keys::LeftShift },
			{ "4D Miner:Sprint", Keys::LeftControl },
			{ "4D Miner:Drop", Keys::G },
			{ "4D Miner:Inventory", Keys::Tab },
			{ "4D Miner:Look 4D", Keys::M },
			{ "4D Miner:Slot 1", Keys::Alpha1 },
			{ "4D Miner:Slot 2", Keys::Alpha2 },
			{ "4D Miner:Slot 3", Keys::Alpha3 },
			{ "4D Miner:Slot 4", Keys::Alpha4 },
			{ "4D Miner:Slot 5", Keys::Alpha5 },
			{ "4D Miner:Slot 6", Keys::Alpha6 },
			{ "4D Miner:Slot 7", Keys::Alpha7 },
			{ "4D Miner:Slot 8", Keys::Alpha8 },
			{ "4D Miner:Chunks Reload", Keys::H }
		}
	},
	{
		KeyBindsScope::STATEGAME,
		{

		}
	},
	{
		KeyBindsScope::STATETITLESCREEN,
		{

		}
	},
	{
		KeyBindsScope::TEXTINPUT,
		{

		}
	}
};

std::vector<std::unordered_map<std::string, KeyBindsScope>> conflicts;

void updateConflicts()
{
	std::unordered_map<Keys, std::vector<std::pair<std::string, KeyBindsScope>>> bindsBySameKey;

	for (const auto& scope : keyBinds) 
		for (const auto& bind : scope.second)
			if(containsVal(namesOrder[scope.first], bind.first))
				bindsBySameKey[bind.second].push_back({ bind.first, scope.first });

	std::vector<std::unordered_map<std::string, KeyBindsScope>> result;
	for (const auto& bind : bindsBySameKey) 
	{
		if (bind.second.size() > 1) 
		{
			std::unordered_map<std::string, KeyBindsScope> bindGroup;
			for (const auto& pair : bind.second)
				bindGroup[pair.first] = pair.second;

			result.push_back(bindGroup);
		}
	}

	conflicts = result;
}

using BindCallback = std::add_pointer<void(GLFWwindow* window, int action, int mods)>::type;
std::map<KeyBindsScope, std::map<std::string, std::vector<BindCallback>>> bindCallbacks;

extern "C" _declspec(dllexport) void addBind(const char* bindName, int defaultKey, int scope, BindCallback callback)
{
	keyBinds[(KeyBindsScope)scope][std::string(bindName)] = (Keys)defaultKey;
	namesOrder[(KeyBindsScope)scope].push_back(std::string(bindName));
	bindCallbacks[(KeyBindsScope)scope][std::string(bindName)].push_back(callback);
}
extern "C" _declspec(dllexport) void hookBind(const char* bindName, int scope, BindCallback callback)
{
	bindCallbacks[(KeyBindsScope)scope][std::string(bindName)].push_back(callback);
}
extern "C" _declspec(dllexport) void triggerBind(const char* bindName, int scope, int action, int mods)
{
	for (const auto& callback : bindCallbacks[(KeyBindsScope)scope][std::string(bindName)])
		callback(nullptr, action, mods);
}

bool justInstalledMod = false;

std::pair<KeyBindsScope, std::string>* curChangingBind = nullptr;

void saveKeybinds()
{
	std::ofstream keybindsFile("keybinds.json");
	if (!keybindsFile.is_open()) return;

	keybindsFile << nlohmann::json(keyBinds).dump(2);

	keybindsFile.close();

	curChangingBind = nullptr;
}

void controlsCloseButtonCallback(void* user)
{
	StateSettings::instanceObj->controlsMenuOpened = false;
	saveKeybinds();
}
void controlsOpenButtonCallback(void* user)
{
	StateSettings::instanceObj->controlsMenuOpened = true;
	curChangingBind = nullptr;
}

std::unordered_map<std::string, std::vector<gui::Element*>> uiStuff;

void keybindButtonCallback(void* user)
{
	auto pair = (std::pair<KeyBindsScope, std::string>*)user;
	curChangingBind = pair;
}

void(__thiscall* StateSettings_init)(StateSettings* self, StateManager& s);
void __fastcall StateSettings_init_H(StateSettings* self, StateManager& s)
{
	StateSettings_init(self, s);

	curChangingBind = nullptr;

	updateConflicts();

	uiStuff.clear();

	self->openControlsButton.text = self->controlsTitleText.text = "Keybinds";
	self->openControlsButton.width += 30;
	self->openControlsButton.callback = controlsOpenButtonCallback; // i was too lazy to write another hook man
	self->controlsContentBox.removeElement(&self->controlsText);

	self->controlsOkButton.text = "Close";
	self->controlsOkButton.callback = controlsCloseButtonCallback; // i was too lazy to write another hook man

	// init some funny ui
	for(auto& scope : keyBinds)
	{
		for(auto& orderedBindName : namesOrder[scope.first])
		{
			Keys bindKey = scope.second[orderedBindName];
			std::pair<std::string, std::string> split = splitBindName(orderedBindName);

			gui::Text* bindName = new gui::Text();
			bindName->text = split.second;
			bindName->shadow = true;
			bindName->alignY(gui::ALIGN_TOP);
			bindName->alignX(gui::ALIGN_CENTER_X);
			bindName->size = 2;
			uiStuff[split.first].push_back(bindName);

			gui::Button* btn = new gui::Button();
			btn->text = KeyToString(bindKey);
			btn->alignY(gui::ALIGN_TOP);
			btn->alignX(gui::ALIGN_CENTER_X);
			btn->width = 40;
			btn->height = 40;
			btn->user = new std::pair<KeyBindsScope, std::string>(scope.first, orderedBindName);
			btn->callback = keybindButtonCallback;
			uiStuff[split.first].push_back(btn);
		}
	}

	int lowestPoint = 0;
	int prevGroupLowestPoint = 0;
	for(auto& ns : uiStuff)
	{
		constexpr int columns = 2;
		const int elementsPerColumn = ns.second.size() / 2 / columns;

		gui::Text* groupTitle = new gui::Text();
		groupTitle->text = std::format("{} Binds", ns.first);
		groupTitle->size = 3;
		groupTitle->shadow = true;
		groupTitle->fancy = true;
		groupTitle->alignX(gui::ALIGN_CENTER_X);
		groupTitle->alignY(gui::ALIGN_TOP);
		groupTitle->offsetY(prevGroupLowestPoint + 20);
		self->controlsContentBox.addElement(groupTitle);
		
		for(int i = 0; i < ns.second.size(); i++)
		{
			bool isBtn = i % 2 == 1;
			int w = 220;
			const int bI = i / 2;
			int column = bI / (elementsPerColumn + 1);
			column = std::min(column, columns - 1);
			int y = (bI % (elementsPerColumn + 1)) * 50 + prevGroupLowestPoint + 40 + 40;
			if (lowestPoint < y)
				lowestPoint = y;
			
			ns.second[i]->offsetY(y - (isBtn ? 12 : 0));

			int x = ((column * 2) - (columns - 1)) * w;
			ns.second[i]->offsetX(x + getWidthOfElement(ns.second[i]) / 2 + (isBtn ? -60 : 0));

			self->controlsContentBox.addElement(ns.second[i]);
		}

		prevGroupLowestPoint = lowestPoint + 40;
	}

	self->controlsContentBox.scrollH = std::max(lowestPoint + 40 - (int)self->controlsContentBox.height, 0);
}

bool isConflicting(const std::string& bind)
{
	for(auto& conflict : conflicts)
		if (conflict.contains(bind))
			return true;

	return false;
}

void(__thiscall* StateSettings_render)(StateSettings* self, StateManager& s);
void __fastcall StateSettings_render_H(StateSettings* self, StateManager& s)
{
	if(self->controlsMenuOpened)
	{
		for (auto& ns : uiStuff)
		{
			for(auto& e : ns.second)
			{
				if(gui::Button* button = dynamic_cast<gui::Button*>(e))
				{
					auto pair = (std::pair<KeyBindsScope, std::string>*)button->user;

					if (curChangingBind != pair)
					{
						button->text = KeyToString(keyBinds[pair->first][pair->second]);
						if (isConflicting(pair->second))
							button->text += ' !';
					}
					else
						button->text = "?";

					int oldW = button->width;
					button->width = button->text.length() * 16 + 24;

					button->xOffset -= (button->width - oldW) / 2;
				}
			}
		}
	}
	StateSettings_render(self, s);
}

struct
{
	bool w; // for xw and zw comb
} additionalKeys;

void callCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods, KeyBindsScope scope)
{
	for (const auto& bind : keyBinds[scope])
		if (key == bind.second && bindCallbacks[scope].contains(bind.first))
			for (const auto& callback : bindCallbacks[scope][bind.first])
				callback(window, action, mods);
}

void(__fastcall* keyInput)(GLFWwindow* window, int key, int scancode, int action, int mods);
void __fastcall keyInput_H(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!curChangingBind)
		callCallbacks(window, key, scancode, action, mods, KeyBindsScope::GLOBAL);

	// im lazy to add hook for StateSettings keyInput
	if(action == GLFW_PRESS && StateSettings::instanceObj->controlsMenuOpened && curChangingBind)
	{
		if(!KeyToString((Keys)key).empty() && (Keys)key != Keys::Escape)
		{
			keyBinds[curChangingBind->first][curChangingBind->second] = (Keys)key;
			curChangingBind = nullptr;
			updateConflicts();
			saveKeybinds();
		}
		else if((Keys)key == Keys::Escape)
			curChangingBind = nullptr;
	}

	keyInput(window, key, scancode, action, mods);
}

void(__thiscall* StateGame_keyInput)(StateGame* self, StateManager& s, int key, int scancode, int action, int mods);
void __fastcall StateGame_keyInput_H(StateGame* self, StateManager& s, int key, int scancode, int action, int mods)
{
	callCallbacks(s.window, key, scancode, action, mods, KeyBindsScope::STATEGAME);
	StateGame_keyInput(self, s, key, scancode, action, mods);
}

void(__thiscall* StateTitleScreen_keyInput)(StateTitleScreen* self, StateManager& s, int key, int scancode, int action, int mods);
void __fastcall StateTitleScreen_keyInput_H(StateTitleScreen* self, StateManager& s, int key, int scancode, int action, int mods)
{
	callCallbacks(s.window, key, scancode, action, mods, KeyBindsScope::STATETITLESCREEN);
	StateTitleScreen_keyInput(self, s, key, scancode, action, mods);
}

void(__thiscall* gui_TextInput_keyInput)(gui::TextInput* self, gui::Window* w, int key, int scancode, int action, int mods);
void __fastcall gui_TextInput_keyInput_H(gui::TextInput* self, gui::Window* w, int key, int scancode, int action, int mods)
{
	callCallbacks(w->getGLFWwindow(), key, scancode, action, mods, KeyBindsScope::TEXTINPUT);
	gui_TextInput_keyInput(self, w, key, scancode, action, mods);
}

bool(__thiscall* Player_keyInput)(Player* self, GLFWwindow* window, World* world, int key, int scancode, int action, int mods);
bool __fastcall Player_keyInput_H(Player* self, GLFWwindow* window, World* world, int key, int scancode, int action, int mods) 
{
	decltype(Player::keys) keysOld = self->keys;
	glm::vec4 forward = self->forward;
	glm::vec4 up = self->up;
	glm::vec4 left = self->left;
	glm::vec4 over = self->over;
	glm::vec4 pos = self->pos;
	float headPos = self->headPos;
	glm::vec4 cameraPos = self->cameraPos;
	bool keepOnEdge = self->keepOnEdge;
	m4::Mat5 orientation = self->orientation;
	float angleToRotate = self->angleToRotate;

	Player_keyInput(self, window, world, key, scancode, action, mods);
	// go fuck yourself. thanks. also sorry people who put some orientation code or keys code in there but no more.
	self->keys = keysOld;
	self->forward = forward;
	self->up = up;
	self->left = left;
	self->over = over;
	self->pos = pos;
	self->headPos = headPos;
	self->cameraPos = cameraPos;
	self->keepOnEdge = keepOnEdge;
	self->orientation = orientation;
	self->angleToRotate = angleToRotate;

	callCallbacks(window, key, scancode, action, mods, KeyBindsScope::PLAYER);

	if (action == GLFW_REPEAT)
		return true;

	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Sprint"])
	{
		self->keys.ctrl = action == GLFW_PRESS;
	}

	if(!self->keys.ctrl)
	{
		if ((keyBinds[KeyBindsScope::PLAYER]["4D Miner:Sprint"] == Keys::LeftControl ||
			keyBinds[KeyBindsScope::PLAYER]["4D Miner:Sprint"] == Keys::RightControl) &&
			(mods & GLFW_MOD_CONTROL) != 0)
		{
			self->keys.ctrl = true;
		}
		if ((keyBinds[KeyBindsScope::PLAYER]["4D Miner:Sprint"] == Keys::LeftShift ||
			keyBinds[KeyBindsScope::PLAYER]["4D Miner:Sprint"] == Keys::RightShift) &&
			(mods & GLFW_MOD_SHIFT) != 0)
		{
			self->keys.ctrl = true;
		}
	}
	

	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Crouch"])
	{
		self->keys.shift = action == GLFW_PRESS;
		self->keepOnEdge = action == GLFW_PRESS;
		world->localPlayerEvent(self, action == GLFW_PRESS ? Packet::C_CROUCH_START : Packet::C_CROUCH_STOP, 0, nullptr);
	}
	if(!self->keys.shift)
	{
		if ((keyBinds[KeyBindsScope::PLAYER]["4D Miner:Crouch"] == Keys::LeftControl ||
			keyBinds[KeyBindsScope::PLAYER]["4D Miner:Crouch"] == Keys::RightControl) &&
			(mods & GLFW_MOD_CONTROL) != 0)
		{
			self->keys.shift = true;
			self->keepOnEdge = true;
			world->localPlayerEvent(self, Packet::C_CROUCH_START, 0, nullptr);
		}
		if ((keyBinds[KeyBindsScope::PLAYER]["4D Miner:Crouch"] == Keys::LeftShift ||
			keyBinds[KeyBindsScope::PLAYER]["4D Miner:Crouch"] == Keys::RightShift) &&
			(mods & GLFW_MOD_SHIFT) != 0)
		{
			self->keys.shift = true;
			self->keepOnEdge = true;
			world->localPlayerEvent(self, Packet::C_CROUCH_START, 0, nullptr);
		}
	}
	

	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Jump"]) {
		self->keys.space = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Left"]) {
		self->keys.a = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Right"]) {
		self->keys.d = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Strafe W+"]) {
		self->keys.e = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Strafe W-"]) {
		self->keys.q = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Drop"] && action == GLFW_PRESS) {
		if(self->hotbar.getSlot(self->hotbar.selectedIndex)->get() != nullptr)
			world->localPlayerEvent(self, Packet::C_ITEM_THROW_HOTBAR, 0, nullptr);
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Inventory"] && action == GLFW_PRESS) {
		if (self->inventoryManager.primary && self->inventoryManager.secondary)
			self->inventoryManager.secondary = nullptr;
		else
		{
			self->targetDamage = 0.f;
			self->inventoryManager.primary = &self->hotbar;
			self->inventoryManager.secondary = &self->inventoryAndEquipment;
			if (self->inventoryManager.primary && self->inventoryManager.secondary)
			{
				self->inventoryManager.craftingMenu.updateAvailableRecipes();
				self->inventoryManager.updateCraftingMenuBox();
			}
			world->localPlayerEvent(self, Packet::C_INVENTORY_OPEN, 0, nullptr);
		}
		self->resetMouse(window);
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Look 4D"]) {
		self->keys.m = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Back"]) {
		self->keys.s = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Forward"]) {
		self->keys.w = action == GLFW_PRESS;
	}
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 1"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 0, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 2"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 1, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 3"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 2, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 4"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 3, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 5"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 4, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 6"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 5, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 7"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 6, nullptr);
	else if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Slot 8"] && action == GLFW_PRESS)
		world->localPlayerEvent(self, Packet::C_HOTBAR_SLOT_SELECT, 7, nullptr);
	if (key == keyBinds[KeyBindsScope::PLAYER]["4D Miner:Chunks Reload"] && action == GLFW_PRESS)
		self->hyperplaneUpdateFlag = true;
	if (key == Keys::R && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL) != 0)
	{
		self->pos.x = (float)self->currentBlock.x + 0.5f;
		self->pos.z = (float)self->currentBlock.z + 0.5f;
		self->pos.w = (float)self->currentBlock.w + 0.5f;

		self->left = glm::vec4(1, 0, 0, 0);
		self->up = glm::vec4(0, 1, 0, 0);
		self->forward = glm::vec4(0, 0, 1, 0);
		self->over = glm::vec4(0, 0, 0, 1);

		self->orientation = m4::Mat5{ 1 };

		self->angleToRotate = 0.0;
		self->hyperplaneUpdateFlag = true;
	}
	if (key == Keys::X)
	{
		self->keys.x = action == GLFW_PRESS;
	}
	if (key == Keys::Z)
	{
		self->keys.z = action == GLFW_PRESS;
	}
	if (key == Keys::Escape && action == GLFW_PRESS)
	{
		if (!self->inventoryManager.primary || !self->inventoryManager.secondary)
			return false;
		self->inventoryManager.secondary = nullptr;
		self->resetMouse(window);
		return true;
	}
	if (key == Keys::W)
		additionalKeys.w = action == GLFW_PRESS;
	if (self->keys.x && self->keys.z && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL) != 0)
	{
		self->pos.x = (float)self->currentBlock.x + 0.5f;
		self->pos.z = (float)self->currentBlock.z + 0.5f;
		self->pos.w = (float)self->currentBlock.w + 0.5f;

		self->left = glm::vec4(1, 0, 0, 0);
		self->up = glm::vec4(0, 1, 0, 0);
		self->forward = glm::vec4(0, 0, 1, 0);
		self->over = glm::vec4(0, 0, 0, 1);

		self->orientation = m4::Mat5{ 1 };

		self->angleToRotate = 0.0;
		self->hyperplaneUpdateFlag = true;
	}
	else if (self->keys.x && additionalKeys.w && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL) != 0)
	{
		self->pos.x = (float)self->currentBlock.x + 0.5f;
		self->pos.z = (float)self->currentBlock.z + 0.5f;
		self->pos.w = (float)self->currentBlock.w + 0.5f;

		self->left = glm::vec4(1, 0, 0, 0);
		self->up = glm::vec4(0, 1, 0, 0);
		self->forward = glm::vec4(0, 0, 0, 1);
		self->over = glm::vec4(0, 0, -1, 0);

		self->orientation = m4::Mat5{ 1 };
		self->orientation[2][2] = 0;
		self->orientation[2][3] = 1;
		self->orientation[3][3] = 0;
		self->orientation[3][2] = -1;

		self->angleToRotate = 0.0;
		self->hyperplaneUpdateFlag = true;
	}
	else if (self->keys.z && additionalKeys.w && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL) != 0)
	{
		self->pos.x = (float)self->currentBlock.x + 0.5f;
		self->pos.z = (float)self->currentBlock.z + 0.5f;
		self->pos.w = (float)self->currentBlock.w + 0.5f;

		self->left = glm::vec4(0, 0, 0, 1);
		self->up = glm::vec4(0, 1, 0, 0);
		self->forward = glm::vec4(0, 0, 1, 0);
		self->over = glm::vec4(-1, 0, 0, 0);

		self->orientation = m4::Mat5{ 1 };
		self->orientation[0][0] = 0;
		self->orientation[0][3] = 1;
		self->orientation[3][0] = -1;
		self->orientation[3][3] = 0;

		self->angleToRotate = 0.0;
		self->hyperplaneUpdateFlag = true;
	}
	if (action == GLFW_RELEASE)
		return true;
	return false;
}

void viewportCallbackFunc(void* user, const glm::ivec4& pos, const glm::ivec2& scroll)
{
	GLFWwindow* window = (GLFWwindow*)user;

	// update the render viewport

	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	glViewport(pos.x, wHeight - pos.y - pos.w, pos.z, pos.w);

	// create a 2D projection matrix from the specified dimensions and scroll position

	glm::mat4 projection2D = glm::ortho(0.0f, (float)pos.z, (float)pos.w, 0.0f, -1.0f, 1.0f);
	projection2D = glm::translate(projection2D, { scroll.x, scroll.y, 0 });

	// update all 2D shaders
	const Shader* textShader = ShaderManager::get("textShader");
	textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* tex2DShader = ShaderManager::get("tex2DShader");
	tex2DShader->use();
	glUniformMatrix4fv(glGetUniformLocation(tex2DShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* quadShader = ShaderManager::get("quadShader");
	quadShader->use();
	glUniformMatrix4fv(glGetUniformLocation(quadShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);
}

gui::ContentBox messageBox;
gui::Button messageBoxOk;
gui::Text messageBoxText;
gui::Text messageBoxText2;

double animTime = 0;
bool closing = false;
bool closed = false;

void messageBoxOkCallback(void* user)
{
	animTime = 0;
	closing = true;
}

bool initializedA = false;

double easeOutElastic(double x)
{
	const double c4 = (2.0 * glm::pi<double>()) / 3.0;

	x = glm::clamp(x, 0.0, 1.0);

	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: pow(2.0, -10.0 * x) * sin((x * 10.0 - 0.75) * c4) + 1.0;
}

void(__thiscall* StateTitleScreen_update)(StateTitleScreen* self, StateManager& s, double dt);
void __fastcall StateTitleScreen_update_H(StateTitleScreen* self, StateManager& s, double dt)
{
	StateTitleScreen_update(self, s, dt);

	if (!initializedA)
	{
		glewExperimental = GL_TRUE;
		glewInit();
	
		self->ui.viewportCallback = viewportCallbackFunc;
		self->ui.viewportUser = s.window;
		self->ui.window = s.window;

		// notify user to check settings->keybinds
		if (justInstalledMod)
		{
			messageBox = gui::ContentBox{};
			messageBox.width = 500;
			messageBox.height = 250;
			messageBox.parent = &self->ui;
			messageBox.alignX(gui::ALIGN_CENTER_X);
			messageBox.alignY(gui::ALIGN_CENTER_Y);
			messageBox.offsetY(-2500);

			messageBoxOk = gui::Button{};
			messageBoxOk.width = 150;
			messageBoxOk.height = 50;
			messageBoxOk.text = "Ok";
			messageBoxOk.alignX(gui::ALIGN_CENTER_X);
			messageBoxOk.alignY(gui::ALIGN_BOTTOM);
			messageBoxOk.offsetY(-20);
			messageBoxOk.callback = messageBoxOkCallback;

			messageBoxText = gui::Text{};
			messageBoxText.text = "You've just installed the 4DKeyBinds mod!";
			messageBoxText.size = 2;
			messageBoxText.shadow = true;
			messageBoxText.alignX(gui::ALIGN_CENTER_X);
			messageBoxText.alignY(gui::ALIGN_TOP);
			messageBoxText.offsetY(20);
			messageBoxText.wrapWidth = 420;

			messageBoxText2 = gui::Text{};
			messageBoxText2.text = "You should check out the Settings for new keybinds.";
			messageBoxText2.size = 2;
			messageBoxText2.shadow = true;
			messageBoxText2.alignX(gui::ALIGN_CENTER_X);
			messageBoxText2.alignY(gui::ALIGN_TOP);
			messageBoxText2.wrapWidth = 420;
			messageBoxText2.offsetY(80);

			messageBox.addElement(&messageBoxText);
			messageBox.addElement(&messageBoxText2);
			messageBox.addElement(&messageBoxOk);

			self->ui.addElement(&messageBox);

			animTime = -0.5;
		}

		initializedA = true;
	}
	if (justInstalledMod)
	{
		if ((animTime < 2.8 && !closing) || (animTime < 0.2 && closing))
		{
			animTime += dt;
			messageBox.yOffset = (int)std::lerp(-1500, 0, !closing ? easeOutElastic(animTime / 2.5 + 0.2) : 1.0 - easeOutElastic(animTime / 2));
		}
		else if (closing && !closed)
		{
			self->ui.removeElement(&messageBox);
			closed = true;
		}
	}
}

DWORD WINAPI Main_Thread(void* hModule)
{
	// Create console window if DEBUG_CONSOLE is defined
#ifdef DEBUG_CONSOLE
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
	
	glfwInit();
	
	// patch out some bullshit code
	{ 
		// patch out shift/crouching check for q and e in Player::update
		unsigned char newBytes[0x4];
		memset(newBytes, 0x90, sizeof(newBytes));
		patchMemory(FUNC_PLAYER_UPDATE + 0x1EC, newBytes, sizeof(newBytes));

		unsigned char newBytes2[0x8];
		memset(newBytes2, 0x90, sizeof(newBytes2));
		patchMemory(FUNC_PLAYER_UPDATE + 0x4B0, newBytes2, sizeof(newBytes2));

		// remove some shit code from Player::keyInput
		// remove slot change cases
		unsigned char newBytes4[0x1b];
		memset(newBytes4, 0x90, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x2F1, newBytes4, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x2D1, newBytes4, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x311, newBytes4, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x331, newBytes4, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x34E, newBytes4, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x36B, newBytes4, sizeof(newBytes4));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x388, newBytes4, sizeof(newBytes4));

		// remove inventory from E press
		unsigned char newBytes5[0x95];
		memset(newBytes5, 0x90, sizeof(newBytes5));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x129, newBytes5, sizeof(newBytes5));

		// even more bs
		unsigned char newBytes10[0x2c];
		memset(newBytes10, 0x90, sizeof(newBytes10));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x79, newBytes10, sizeof(newBytes10));

		unsigned char newBytes11[0x11];
		memset(newBytes11, 0x90, sizeof(newBytes11));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x113, newBytes11, sizeof(newBytes11));

		unsigned char newBytes12[0x12];
		memset(newBytes12, 0x90, sizeof(newBytes12));
		patchMemory(FUNC_PLAYER_KEYINPUT + 0x2AC, newBytes12, sizeof(newBytes12));


		// FIX FOR MOVEMENT!!!!
		// remove else if (replace it with an if) in A and D movement
		unsigned char newBytes13[0x2];
		memset(newBytes13, 0x90, sizeof(newBytes13));
		patchMemory(FUNC_PLAYER_UPDATE + 0x47B, newBytes13, sizeof(newBytes13));
	}

	// load keybinds
	if (!std::filesystem::exists("keybinds.json"))
	{
		justInstalledMod = true;

		// create keybinds.json
		saveKeybinds();
	}
	else
	{
		std::ifstream keybindsFile("keybinds.json");
		if(keybindsFile.is_open())
		{
			nlohmann::json keybindsJson = nlohmann::json::parse(keybindsFile);
			keybindsFile.close();
			
			keyBinds = keybindsJson;
		}
	}

	Hook(reinterpret_cast<void*>(FUNC_STATESETTINGS_INIT), reinterpret_cast<void*>(&StateSettings_init_H), reinterpret_cast<void**>(&StateSettings_init));
	Hook(reinterpret_cast<void*>(FUNC_STATESETTINGS_RENDER), reinterpret_cast<void*>(&StateSettings_render_H), reinterpret_cast<void**>(&StateSettings_render));
	Hook(reinterpret_cast<void*>(FUNC_STATETITLESCREEN_UPDATE), reinterpret_cast<void*>(&StateTitleScreen_update_H), reinterpret_cast<void**>(&StateTitleScreen_update));
	
	Hook(reinterpret_cast<void*>(FUNC_PLAYER_KEYINPUT), reinterpret_cast<void*>(&Player_keyInput_H), reinterpret_cast<void**>(&Player_keyInput));
	Hook(reinterpret_cast<void*>(FUNC_STATEGAME_KEYINPUT), reinterpret_cast<void*>(&StateGame_keyInput_H), reinterpret_cast<void**>(&StateGame_keyInput));
	Hook(reinterpret_cast<void*>(FUNC_STATETITLESCREEN_KEYINPUT), reinterpret_cast<void*>(&StateTitleScreen_keyInput_H), reinterpret_cast<void**>(&StateTitleScreen_keyInput));
	Hook(reinterpret_cast<void*>(FUNC_GUI_TEXTINPUT_KEYINPUT), reinterpret_cast<void*>(&gui_TextInput_keyInput_H), reinterpret_cast<void**>(&gui_TextInput_keyInput));
	// that address isnt in addresses.h yet. but its a function used directly in the glfwKeyCallback. and its called keyCallback lol
	Hook(reinterpret_cast<void*>(base + idaOffsetFix(0x9E490)), reinterpret_cast<void*>(&keyInput_H), reinterpret_cast<void**>(&keyInput));

	EnableHook(0);

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD _reason, LPVOID lpReserved)
{
	if (_reason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main_Thread, hModule, 0, NULL);
	return TRUE;
}
