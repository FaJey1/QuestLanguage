#ifndef __ARRAYBYFAJEY__
#define __ARRAYBYFAJEY__
#include <iostream>
#include <string>

using namespace std;

template<typename T1>
class ArrayByFaJey {

private:
	template<typename T2>
	class Element
	{
	public:
		T2 value;
		Element* v_next;
		Element* v_previous;
		Element(T2 value = T2(), Element* next = nullptr, Element* prev = nullptr) {
			this->value = value;
			this->v_next = next;
			this->v_previous = prev;
		}
	};

	int array_size;
	Element<T1>* head, * tail;

public:
	ArrayByFaJey();
	~ArrayByFaJey();
	int get_size();

	void add_tail(T1 element);
	void add_head(T1 element);
	void add_element(T1 element, int index);

	void remove_tail();
	void remove_head();
	void remove_element(int index);

	T1& operator[](const int index);
};

template<typename T1>
ArrayByFaJey<T1>::ArrayByFaJey() {
	this->array_size = 0;
	this->head = nullptr;
	this->tail = nullptr;
}

template<typename T1>
ArrayByFaJey<T1>::~ArrayByFaJey() {


}

template<typename T1>
int ArrayByFaJey<T1>::get_size() {
	return this->array_size;
}

template<typename T1>
void ArrayByFaJey<T1>::add_tail(T1 element) {
	if (array_size == 0) {
		head = new Element<T1>(element);
		tail = head;
	}
	else {
		Element<T1>* current = new Element<T1>(element, tail->v_previous, tail);
		tail->v_next = current;
		tail = current;
	}

	this->array_size++;
}

template<typename T1>
void ArrayByFaJey<T1>::add_head(T1 element) {
	if (array_size == 0) {
		head = new Element<T1>(element);
		tail = head;
	}
	else {
		Element<T1>* current = new Element<T1>(element, head, head->v_next);
		head->v_previous = current;
		head = current;
	}

	this->array_size++;
}

template<typename T1>
void ArrayByFaJey<T1>::add_element(T1 element, int index) {
	if (index >= this->array_size || index < 0) {
		return;
	}

	if (index == 0) {
		this->add_head(element);
		return;
	}

	if (index == array_size - 1) {
		this->add_tail(element);
		return;
	}

	int count = 0;

	Element<T1>* new_element;
	Element<T1>* search_element = this->head;

	while (search_element != nullptr) {
		if (count == index - 1) {
			//cout << "pos: " << search_element->value << "\n";
			break;
		}

		search_element = search_element->v_next;
		count++;
	}
	new_element = new Element<T1>(element, search_element->v_next, search_element);
	search_element->v_next = new_element;


	this->array_size++;
}

template<typename T1>
void ArrayByFaJey<T1>::remove_tail() {

}

template<typename T1>
void ArrayByFaJey<T1>::remove_head() {

}

template<typename T1>
void ArrayByFaJey<T1>::remove_element(int index) {

}

template<typename T1>
T1& ArrayByFaJey<T1>::operator[](const int index) {
	int count = 0;
	Element<T1>* element = this->head;

	while (element != nullptr) {
		if (count == index) {
			return element->value;
		}

		element = element->v_next;
		count++;
	}
}
#endif

