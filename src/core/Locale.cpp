#include "Locale.hpp"

static const char *delim = ".";

struct TextEntry{
	String name;
	String value;
	TextEntry *next;
};

Locale::Locale() : entries(nullptr) {}

Locale::~Locale(){
	while(entries){
		TextEntry *next = entries->next;
		delete entries;
		entries = next;
	}
}
bool Locale::ProcessJSONValue(const char *prefix, const rapidjson::Value &object) {
	for(jsonutils::ConstIterator i = object.MemberBegin(); i != object.MemberEnd(); ++i){
		const rapidjson::Value &name = i->name;
		const rapidjson::Value &value = i->value;
		if(!name.IsString()){
			Log(LEVEL_ERROR, "Item name is not a string");
			return false;
		}
		if(value.IsObject()){
			if(!ProcessJSONValue(String::Format("%s%s%s",
					prefix, name.GetString(), delim), value))
				return false;
		}else{
			if(!value.IsString()){
				Log(LEVEL_ERROR, "Item value is not a string");
				return false;
			}
			TextEntry *entry = new TextEntry();
			entry->name = String::Format("%s%s", prefix, name.GetString());
			entry->value = value.GetString();
			entry->next = entries;
			entries = entry;
		}
	}
	return true;
}

bool Locale::Load(SDL_RWops *file){
	JSONResource res;
	return
		res.Load(file) &&
		ProcessJSONValue("", res.document);
}

const char *Locale::operator[](const char *name) const{
	for(TextEntry *entry = entries; entry; entry = entry->next)
		if(entry->name == name)
			return entry->value;
	LogF(LEVEL_WARNING, "Value \"%s\" not found", name);
	return "";
}
