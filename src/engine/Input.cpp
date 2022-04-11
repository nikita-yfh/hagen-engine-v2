#include "Input.hpp"

namespace input{

static const char *mouseKeys[] = {
	"Mouse left",
	"Mouse middle",
	"Mouse right",
	"Mouse X1",
	"Mouse X2"
};

keycode GetMousePressedCode(){
	uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
	if(mouseState & (1 << 0))
		return MOUSE_KEYS + 0;
	if(mouseState & (1 << 1))
		return MOUSE_KEYS + 1;
	if(mouseState & (1 << 2))
		return MOUSE_KEYS + 2;
	if(mouseState & (1 << 3))
		return MOUSE_KEYS + 3;
	if(mouseState & (1 << 4))
		return MOUSE_KEYS + 4;
	return NOT_PRESSED;
}
keycode GetPressedCode(){
	const uint8_t *keyboardState = SDL_GetKeyboardState(nullptr);
	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
		if(keyboardState[i])
			return i;
	return GetMousePressedCode();
}
const char *KeyToStr(keycode key){
	if(key == NOT_PRESSED)
		return "-";
	else if(key < MOUSE_KEYS){
		const char *str = SDL_GetScancodeName((SDL_Scancode)key); if(str != nullptr) return str;
		return "Unknown key";
	}else if(key < KEY_COUNT){
		const char *str = mouseKeys[key - MOUSE_KEYS];
		return str;
	}
	return "Unknown key";
}
keycode StrToKey(const char *name){
	keycode key = SDL_GetScancodeFromName(name);
	if(key != SDL_SCANCODE_UNKNOWN)
		return key;
	for(int i = 0; i < 5; i++)
		if(strcmp(name, mouseKeys[i]) == 0)
			return MOUSE_KEYS + i;
	return NOT_PRESSED;
}

InputConfig::InputConfig() {
	keys = nullptr;
	keyCount = 0;
}
InputConfig::~InputConfig(){
	if(keys)
		delete [] keys;
}
bool InputConfig::FromJSON(const rapidjson::Value &value){
	if(!jsonutils::CheckObject(value) || !jsonutils::CheckValue(value, "keys"))
		return false;
	const rapidjson::Value &array = value["keys"];
	if(!jsonutils::CheckArray(array))
		return false;
	keyCount = array.Size();
	keys = new Key[keyCount];
	for(int i = 0; i < keyCount; i++){
		const rapidjson::Value &value = array[i];
		const char *key;
		if(!jsonutils::CheckObject(value) ||
				!jsonutils::GetMember(value, "id", keys[i].name) ||
				!jsonutils::GetMember(value, "defaultKey", key))
			return false;
		keys[i].key = keys[i].defaultKey = StrToKey(key);
	}
	return true;
}
bool InputConfig::LoadSettings(const rapidjson::Value &value){
	if(!jsonutils::CheckValue(value, "keyBindings"))
		return false;
	const rapidjson::Value &array = value["keyBindings"];
	if(!jsonutils::CheckArray(array))
		return false;
	size_t count = array.Size();
	if(keyCount != count) {
		Log(LEVEL_ERROR, "Binding key count does not match");
		return false;
	}
	for(int i = 0; i < keyCount; i++){
		const char *keyStr;
		if(!jsonutils::GetArrayMember(array, i, keyStr))
			return false;
		keys[i].key = StrToKey(keyStr);
	}
	return true;
}
void InputConfig::SaveSettings(rapidjson::Value &value, jsonutils::Allocator &allocator) const{
	rapidjson::Value array(rapidjson::kArrayType);
	for(int i = 0; i < keyCount; i++)
		array.PushBack(jsonutils::StringType(KeyToStr(keys[i].key)), allocator);
	value.AddMember("keyBindings", array, allocator);
}
keycode InputConfig::GetKey(const char *str) const {
	for(int i = 0; i < keyCount; i++)
		if(keys[i].name == str)
			return keys[i].key;
	return NOT_PRESSED;
}
bool InputConfig::SetDefault(){
	for(int i = 0; i < keyCount; i++)
		keys[i].key = keys[i].defaultKey;
	return true;
}

}

