#include "EventManager.h"

EventManager::EventManager() : m_hasFocus(true) { LoadBindings(); }

EventManager::~EventManager()
{
	for (auto &itr : m_bindings) {
		delete itr.second; 
		itr.second = nullptr; 
	}
}

bool EventManager::AddBinding(Binding* l_binding)
{
	//std::cout << l_binding->m_name << std::endl;
	if (m_bindings.find(l_binding->m_name) != m_bindings.end())
		return false; 
	return m_bindings.emplace(l_binding->m_name, l_binding).second; 
}

bool EventManager::RemoveBinding(std::string l_name)
{
	auto itr = m_bindings.find(l_name); 
	// if the iterator isn't found, return false
	if (itr == m_bindings.end()) { return false; }
	//experiment with this later; 
	delete itr->second; 
	m_bindings.erase(itr);
	return true; 
}

void EventManager::SetFocus(const bool & l_focus)
{
}

void EventManager::HandleEvent(sf::Event & l_event) // handling SFML events 
{
	for (auto &b_itr : m_bindings) { //iterating through m_bindings( which is an unordered map composed of strings, and Binding*
									// Binding* points to struct containing Events and eventDetails. 
		Binding* bind = b_itr.second; 
		// makes a variable (bind).pointing to the current iteration of m_bindings second slot. 
		for (auto &e_itr : bind->m_events) { // iterates through bind's m_events, which is of type Events. Events is a vector pair
											// of EventType and EventInfo
			
			EventType sfmlEvent = (EventType)l_event.type; // casts the sf::event.type enum as EnumType to the variable sfmlEvent
			if (e_itr.first != sfmlEvent) { continue;} // checks to see if the sfml event type is the same as the current first element of the current iteration through a list of stored eventsz
														// i think this is so that the program doesnt need to run if statments checking to see if its every type of event. 
														// alright tbh i dont really get it, its takes the curent event then tests it against every event it has stored, to save time testing the follow events it has saved? i dont really get if its meant to be saving time and if so idk how it does, and if it doesnt save time idk what purpose this line holds 
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) { // checking to see if its a keyboard event 
				if (e_itr.second.m_code == l_event.key.code) { //if the current iteration through bind->m_events is equal to the key code then
					if (bind->m_details.m_keyCode != -1) {// test to see if the current iteration through m_binding's Binding* keycode does not equal -1, what does it mean if its -1, why would it be -1? idk. maybe -1 is something to mean like "dont overbind me!!" idk man 
						bind->m_details.m_keyCode = e_itr.second.m_code; //sets bind m_details to the current iteration through the 
					}
					bind->c++;
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) {
				if (e_itr.second.m_code == l_event.mouseButton.button) {
					//matching event/keystroke.
					//increase count 
					bind->m_details.m_mouse.x = l_event.mouseButton.x; 
					bind->m_details.m_mouse.y = l_event.mouseButton.y; 
					if (bind->m_details.m_keyCode != -1) {
						bind->m_details.m_keyCode = e_itr.second.m_code; 
					}
					bind->c++; 
					break;
				}
			
			}
			else {
				if (sfmlEvent == EventType::MouseWheel) {
					bind->m_details.m_mouseWheelDelta = l_event.mouseWheel.delta; 
				}
				else if (sfmlEvent == EventType::WindowResized) {
					bind->m_details.m_size.x = l_event.size.width; 
					bind->m_details.m_size.y = l_event.size.height; 
				}
				else if (sfmlEvent == EventType::TextEntered) {
					bind->m_details.m_textEntered = l_event.text.unicode; 
				}
				bind->c++; 
			}
		}
	}
}

void EventManager::Update()
{
	if (!m_hasFocus) { return; }
	for (auto& binding_itr : m_bindings) {
		Binding* bind = binding_itr.second;
		for (auto& events_itr : bind->m_events) {
			switch (events_itr.first) {
			case(EventType::Keyboard) : 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(events_itr.second.m_code))) {
					if (bind->m_details.m_keyCode != -1) {
						bind->m_details.m_keyCode = events_itr.second.m_code; 
					}
					bind->c++; 
				}
				break;
			case(EventType::Mouse) : 
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(events_itr.second.m_code))) {
					if (bind->m_details.m_keyCode != -1) {
						bind->m_details.m_keyCode = events_itr.second.m_code; 
					}
					bind->c++; 
				}
				break; 
			}
		}
		if (bind->m_events.size() == bind->c) {
			auto callItr = m_callbacks.find(bind->m_name);
			if (callItr != m_callbacks.end()) {
				callItr->second(&bind->m_details);
			}
		}
		bind->c = 0; 
		bind->m_details.Clear(); 
	}
}
//private 
void EventManager::LoadBindings()
{
	std::string delimiter = ":";

	std::ifstream bindings;
	bindings.open("keys.cfg");
	if (!bindings.is_open()) {
		std::cout << "! Failed loading keys.cfg ! " << std::endl; 
		return; 
	}
	std::string line; 
	while (std::getline(bindings, line)) {
		std::stringstream keystream(line);
		std::string callbackName; 
		keystream >> callbackName; //writes the first word of keystream to callBackName
		Binding* bind = new Binding(callbackName); // allocates a new Binding object on the heap 
		while (!keystream.eof()) {// im guessing this means while keystream doesn't equal the end of the current line
			std::string keyval; 
			keystream >> keyval; // puts the next "word" into the keyval. ex say the line is Move 9:0, it would be inputing 9:0 into the keyval string 
			int start = 0; 
			int end = keyval.find(delimiter);
			if (end == std::string::npos) {
				delete bind; //deallocates bind from the heap 
				bind = nullptr;
				break; 
			}
			EventType type = EventType(stoi(keyval.substr(start, end - start)));// setting the event type to the number before the delimiter
																				// but after the first word so in the example from line ~142, it would be 9
			int code = stoi(
				keyval.substr(
					end + delimiter.length(), // the starting position is after the position of the colon
					keyval.find(delimiter, end + delimiter.length())  // the size is the position where the first delimiter is found starting from after where the first delimiter is found 
								//aka until the next delimiter in the line, or till there is a space in the line 
				)
			); 
			EventInfo eventInfo; 
			eventInfo.m_code = code; 
			bind->BindEvent(type, eventInfo);
		}
		if (!AddBinding(bind)) { delete bind; }
		bind = nullptr; 
	}
	bindings.close(); 
}
