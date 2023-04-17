// exclusive.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>

#include "Include/exclusive.hpp"

using namespace std;

class Sample
{
public:
	Sample(const string& name, int value = 0) : name_(name), value_(value) {}

	const string& Name() const { return name_; }
	int Value() const { return value_; }

	void SetName(const string& name) { name_ = name; }
	void Inc(void) { ++value_; }
	void Dec(void) { --value_; }

private:
	string name_;
	int value_;
};

using ExclusiveSamplePtr = exclusive_ptr<Sample>;
using ExclusiveSampleObj = exclusive_obj<Sample>;

ostream& operator <<(ostream & out, const ExclusiveSamplePtr& nameValue)
{
	// Explicitly acquire resource since we need to access multiple attributes
	auto lockedNameValue = nameValue.acquire();
	out << lockedNameValue->Name() << ": " << lockedNameValue->Value() << endl;
	return out;
}

ostream& operator <<(ostream& out, const ExclusiveSampleObj& nameValue)
{
	// Explicitly acquire resource since we need to access multiple attributes
	auto lockedNameValue = nameValue.acquire();
	out << lockedNameValue->Name() << ": " << lockedNameValue->Value() << endl;
	return out;
}

void SampleExclusivePtr()
{
	//ExclusiveNameValuePtr nameValue(new NameValue("Sample"));
	ExclusiveSamplePtr nameValue{ "Sample" };
	
	cout << "Implicit lock example" << endl;
	nameValue->Inc();
	nameValue->Inc();
	cout << endl;

	cout << "Explicit lock example" << endl;
	cout << nameValue << endl;
}

void SampleExclusiveObj()
{
	ExclusiveSampleObj nameValue{ "Sample" };

	cout << "Implicit lock example" << endl;
	nameValue->Inc();
	nameValue->Inc();
	cout << endl;

	cout << "Explicit lock example" << endl;
	cout << nameValue << endl;
}

int main()
{
	cout << "Start sample" << endl << endl;

	cout << "---------- Using exclusive_ptr" << endl;
	SampleExclusivePtr();

	cout << "---------- Using exclusive_obj" << endl;
	SampleExclusiveObj();

	cout << "End sample" << endl;
}
