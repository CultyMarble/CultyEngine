#include <CultyEngine/Inc/CultyEngine.h>

using namespace CultyEngine;
using namespace CultyEngine::Core;

static int sUniqueID = 0;
class Student
{
public:
	Student(const std::string& name)
		: mName(name), mID(++sUniqueID) {}

	Student(const std::string& name, int id)
		: mName(name), mID(id) {}

private:
	int mID = {};
	std::string mName = {};
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 200);

	std::vector<Student*> students;

	// Test Create
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i));
		students.push_back(newStudent);
	}

	// Test Delete
	for (uint32_t i = 0; i < 20; ++i)
	{
		auto& student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	// Test Add
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* student = studentPool.New("Student_New_" + std::to_string(i), i);
		students.push_back(student);
	}

	for (auto& student: students)
		studentPool.Delete(student);

	students.clear();

	return 0;
}