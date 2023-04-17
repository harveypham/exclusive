exclusive
=========

Header-only implementation of exclusive_obj and exclusive_ptr that turns a class
to thread-safe (exlusive). The differences between exclusive_obj and exclusive_ptr
are how the underlining class is defined within the wrapper classes.

Assuming that the following class is defined:

```CPP
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
```

Basic Usage
-----------

1. Declaring resource
```CPP
using ExcSamplePtr = exclusive_ptr<Sample>;

ExcSamplePtr sample{ "Sample" }; // Construction using the same syntax as class Sample
```

2. Accesing resource directly with implicit locking
```CPP
// Each invocation implicitly acquires lock, invoke the underline method, then
// release lock
sample->Inc();
std::string name = sample.Name();
int value = sample.Value();
```

3. Explictly acquire the underling resource
```CPP
auto lockedSample = sample.acquire(); // Lock 
// Access locked resource via lockedSample
lockedSample->Inc();
std::cout << lockedSample->Name() << ": " << lockedSample->Value() << endl;
// Unlock when lockedSample is out of scope
```

Using a different locking mechanism
-----------------------------------
By default, the wrapper classes use `std::mutex` and `std::lock_guard<mutex>` to
handle locking. Users can specify different locking mechanism:
```CPP
// Using ATL critical section instead
locked_ptr<Sample, ATL::CCriticalSection, ATL::CCritSecLock> sample2 { "Sample 2" };
```