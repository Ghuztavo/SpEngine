#include <SpEngine/Inc/SpEngine.h>

using namespace SpEngine;
using namespace SpEngine::Core;

static int sUniqueId = 0;
class Student
{
public:
	Student()
	{
		mId = ++sUniqueId;
		mMark = 60 + (rand() % 41);
		mName = "NoName_" + std::to_string(mId);
	}
	Student(const std::string& name)
		: mName(name)
	{
		mId = ++sUniqueId;
		mMark = 60 + (rand() % 41);
	}
	Student(const std::string& name, int mark)
		: mName(name), mMark(mark)
	{
		mId = ++sUniqueId;
	}
	void Log()
	{
		LOG("Name: %s, - ID: %d - Mark: %d", mName.c_str(), mId, mMark);
	}
private:
	int mId = 0;
	int mMark = 0;
	std::string mName;
};

// Assignment
class Weapon
{
public:
	Weapon()
	{
		mId = ++sUniqueId;
		mDamage = 10 + (rand() % 91);
		mName = "NoName_" + std::to_string(mId);
	}
	Weapon(const std::string& name, int damage)
		: mName(name), mDamage(damage)
	{
		mId = ++sUniqueId;
	}

	void Log()
	{
		LOG("Name: %s, - ID: %d - Damage: %d", mName.c_str(), mId, mDamage);
	}
private:
	std::string mName;
	int mId = 0;
	int mDamage = 0;
};

class Enemy
{
public:
	Enemy()
	{
		mId = ++sUniqueId;
		mHealth = 100 + (rand() % 901);
		mName = "NoName_" + std::to_string(mId);
	}
	void Log()
	{
		LOG("Name: %s, - ID: %d - Health: %d", mName.c_str(), mId, mHealth);
	}
private:
	std::string mName;
	int mId = 0;
	int mHealth = 0;
};

class Vehicle
{
public:
	Vehicle()
	{
		mId = ++sUniqueId;
		mSpeed = 50 + (rand() % 151);
		mName = "NoName_" + std::to_string(mId);
	}
	void Log()
	{
		LOG("Name: %s, - ID: %d - Speed: %d", mName.c_str(), mId, mSpeed);
	}
private:
	std::string mName;
	int mId = 0;
	int mSpeed = 0;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	//TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);

	//std::vector<Student*> students;

	//// test new
	//for (uint32_t i = 0; i < 50; ++i)
	//{
	//	std::string newStudentName = "NewStudent" + std::to_string(i);
	//	Student* newStudent = studentPool.New(newStudentName, 75);
	//	newStudent->Log();
	//	students.push_back(newStudent);
	//}

	//// test delete
	//for (uint32_t i = 0; i < 30; ++i)
	//{
	//	Student* student = students.back();
	//	studentPool.Delete(student);
	//	students.pop_back();
	//}

	//// test add more
	//for (uint32_t i = 0; i < 20; ++i)
	//{
	//	Student* newStudent = studentPool.New();
	//	newStudent->Log();
	//	students.push_back(newStudent);
	//}

	//// cleanup
	//for (Student* student : students)
	//{
	//	student->Log();
	//	studentPool.Delete(student);
	//}
	//students.clear();

	// Assignment
	TypedAllocator<Weapon> weaponPool = TypedAllocator<Weapon>("WeaponPool", 100);
	TypedAllocator<Enemy> enemyPool = TypedAllocator<Enemy>("EnemyPool", 100);
	TypedAllocator<Vehicle> vehiclePool = TypedAllocator<Vehicle>("VehiclePool", 100);

	// Create the objects
	std::vector<Weapon*> weapons;
	for (uint32_t i = 0; i < 70; ++i)
	{
		std::string weaponName = "Weapon" + std::to_string(i);
		Weapon* newWeapon = weaponPool.New(weaponName, 20 + (rand() % 81));
		newWeapon->Log();
		weapons.push_back(newWeapon);
	}

	std::vector<Enemy*> enemies;
	for (uint32_t i = 0; i < 70; ++i)
	{
		Enemy* newEnemy = enemyPool.New();
		newEnemy->Log();
		enemies.push_back(newEnemy);
	}

	std::vector<Vehicle*> vehicles;
	for (uint32_t i = 0; i < 70; ++i)
	{
		Vehicle* newVehicle = vehiclePool.New();
		newVehicle->Log();
		vehicles.push_back(newVehicle);
	}

	// delete some objects
	for (uint32_t i = 0; i < 40; ++i)
	{
		Weapon* weapon = weapons.back();
		weaponPool.Delete(weapon);
		weapons.pop_back();
	}

	for (uint32_t i = 0; i < 40; ++i)
	{
		Enemy* enemy = enemies.back();
		enemyPool.Delete(enemy);
		enemies.pop_back();
	}

	for (uint32_t i = 0; i < 40; ++i)
	{
		Vehicle* vehicle = vehicles.back();
		vehiclePool.Delete(vehicle);
		vehicles.pop_back();
	}

	// add more objects
	for (uint32_t i = 0; i < 30; ++i)
	{
		Weapon* newWeapon = weaponPool.New();
		newWeapon->Log();
		weapons.push_back(newWeapon);
	}

	for (uint32_t i = 0; i < 30; ++i)
	{
		Enemy* newEnemy = enemyPool.New();
		newEnemy->Log();
		enemies.push_back(newEnemy);
	}

	for (uint32_t i = 0; i < 30; ++i)
	{
		Vehicle* newVehicle = vehiclePool.New();
		newVehicle->Log();
		vehicles.push_back(newVehicle);
	}

	// cleanup
	for (Weapon* weapon : weapons)
	{
		weapon->Log();
		weaponPool.Delete(weapon);
	}
	weapons.clear();

	for (Enemy* enemy : enemies)
	{
		enemy->Log();
		enemyPool.Delete(enemy);
	}
	enemies.clear();

	for (Vehicle* vehicle : vehicles)
	{
		vehicle->Log();
		vehiclePool.Delete(vehicle);
	}
	vehicles.clear();

	return 0;
}
