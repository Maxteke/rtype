/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** Packet
*/

#include "Packet.hpp"


/*
	Room name
	List Rooms
	Packet :
*/

void displayRoom(const room &e, bool toJoin)
{
	std::cout << "\t" << ((toJoin)?"toJoin":"") << "Room" << "\n";
	std::cout << "\t\t_name\t\t" << e.name << std::endl;
	std::cout << "\t\t_port\t\t" << e.port << "\n";
}

void displayRooms(const std::vector<room> &t)
{
	for (int i = 0; i < t.size(); i++)
		displayRoom(t[i]);
}

void displayEntity(const entity &e)
{
	std::cout << "\tEntity" << "\n";
	std::cout << "\t\t_name\t\t" << e.name << std::endl;
	std::cout << "\t\t_x\t\t" << e.x << "\n";
	std::cout << "\t\t_y\t\t" << e.y << "\n";
	std::cout << "\t\t_health\t\t" << e.health << "\n";
	std::cout << "\t\t_score\t\t" << e.score << "\n";
	std::cout << "\t\t_weapon\t\t" << e.weapon << "\n";
}

void displayEntities(const std::vector<entity> &t)
{
	for (int i = 0; i < t.size(); i++)
		displayEntity(t[i]);
}

int numberFromFourChar(const char *s, int index)
{
	int res = 0;
	res += s[index] * 1000;
	res += s[index+1] * 100;
	res += s[index+2] * 10;
	res += s[index+3];
	return res;
}

void fourCharFromNumber(std::vector<char> *fill, int index, int nb)
{
	if (!nb)
		return;
	(*fill)[index+0] = (int)(nb / 1000);
	(*fill)[index+1] = (int)((nb%1000) /  100);
	(*fill)[index+2] = (int)((nb%1000%100) /  10);
	(*fill)[index+3] = (int)((nb%1000%100%10) );
}

int numberFromEightChar(const char *s, int index)
{
	int res = 0;
	res += s[index] * 10000000;
	res += s[index+1] * 1000000;
	res += s[index+2] * 100000;
	res += s[index+3] * 10000;
	res += s[index+4] * 1000;
	res += s[index+5] * 100;
	res += s[index+6] * 10;
	res += s[index+7];
	return res;
}

void eightCharFromNumber(std::vector<char> *fill, int index, int nb)
{
	if (!nb)
		return;
	(*fill)[index+0] = (int)(nb / 10000000);
	(*fill)[index+1] = (int)((nb%10000000) /  1000000);
	(*fill)[index+2] = (int)((nb%10000000%1000000) /  100000);
	(*fill)[index+3] = (int)((nb%10000000%1000000%100000) / 10000);
	(*fill)[index+4] = (int)((nb%10000000%1000000%100000%10000) / 1000);
	(*fill)[index+5] = (int)((nb%10000000%1000000%100000%10000%1000) / 100);
	(*fill)[index+6] = (int)((nb%10000000%1000000%100000%10000%1000%100) / 10);
	(*fill)[index+7] = (int)((nb%10000000%1000000%100000%10000%1000%100%10));
}



Packet::Packet()
 :	_type(PACKET_PING), _dataSize(0), _entity(entity()), _toJoin(room()), _createdPort(0)
{
	_username = "";
	_toCreate = "";
	_shooter = "";
}

Packet::~Packet()
{
}

void Packet::setUsername(const std::string &username)
{
	_username = username;
}

void Packet::setEntity(const entity &entity)
{
	_entity.name = entity.name;
	_entity.x = entity.x;
	_entity.y = entity.y;
	_entity.health = entity.health;
	_entity.score = entity.score;
	_entity.weapon = entity.weapon;
}


void Packet::setRoomToJoin(const room &room)
{
	_toJoin.name = room.name;
	_toJoin.port = room.port;
}

void Packet::setToCreate(const std::string &ToCreate)
{
	_toCreate = ToCreate;
}

void Packet::fromString(const std::string &str, char prov)
{
	if (str.size() < PACKET_SIZE_PACKET)
		std::cerr << "inside Packet::fromString(const std::string &s, char p)\tPacket.cpp line 44\nWARNING : string size is inferior to packet size, segfault should follow." << std::endl;
	fromCharString(str.c_str(), prov);
}

void Packet::fromCharString(const char *str, char prov)
{
	try {
		_type = (PacketType)str[PACKET_INDEX_TYPE];
		_dataSize = str[PACKET_INDEX_DATASIZE] * 1000\
					+ str[PACKET_INDEX_DATASIZE + 1] * 100\
					+ str[PACKET_INDEX_DATASIZE + 2] * 10\
					+ str[PACKET_INDEX_DATASIZE + 3];

		if (_type == PACKET_USERNAME)
			_username = backwardsString(&str[PACKET_INDEX_DATA]);
		if (_type == PACKET_SPAWN) {
			_entity.name = backwardsString(&str[PACKET_INDEX_DATA]);
			_entity.x = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME);
			_entity.y = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + PACKET_SIZE_POS);
			_entity.health = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 2*PACKET_SIZE_POS);
			_entity.score = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 3*PACKET_SIZE_POS);
			_entity.weapon = backwardsString(&str[PACKET_INDEX_DATA + PACKET_SIZE_NAME + 4*PACKET_SIZE_POS]);
		}
		if (_type == PACKET_UPDATE) {
			_entities.clear();
			for (int i = 0; i < _dataSize; i++) {
				entity *tmp = new entity();
				tmp->name = backwardsString(&str[PACKET_INDEX_DATA + (i * PACKET_SIZE_ENTITY)]);
				tmp->x = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + (i * PACKET_SIZE_ENTITY));
				tmp->y = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + PACKET_SIZE_POS + (i * PACKET_SIZE_ENTITY));
				tmp->health = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 2*PACKET_SIZE_POS + (i * PACKET_SIZE_ENTITY));
				tmp->score = numberFromFourChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 3*PACKET_SIZE_POS + (i * PACKET_SIZE_ENTITY));
				tmp->weapon = backwardsString(&str[PACKET_INDEX_DATA + PACKET_SIZE_NAME + 4*PACKET_SIZE_POS] + (i * PACKET_SIZE_ENTITY));
				_entities.push_back(*tmp);
			}
		}
		if (_type == PACKET_LIST_ROOMS) {
			_rooms.clear();
			for (int i = 0; i < _dataSize; i++) {
				room *tmp = new room();
				tmp->name = backwardsString(&str[PACKET_INDEX_DATA + (i * PACKET_SIZE_ROOM)]);
				tmp->port = numberFromEightChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME + (i * PACKET_SIZE_ROOM));
				_rooms.push_back(*tmp);
			}
		}
		if (_type == PACKET_JOIN_ROOM) {
			_toJoin.name = backwardsString(&str[PACKET_INDEX_DATA]);
			_toJoin.port = numberFromEightChar(str, PACKET_INDEX_DATA + PACKET_SIZE_NAME);

		}
		if (_type == PACKET_CREATE_ROOM)
			_toCreate = backwardsString(&str[PACKET_INDEX_DATA]);
		if (_type == PACKET_CREATED_ROOM)
			_createdPort = numberFromEightChar(str, PACKET_INDEX_DATA);
		if (_type == PACKET_SHOOT)
			_shooter = backwardsString(&str[PACKET_INDEX_DATA]);
	}
	catch(const std::exception& e) {
		std::cerr << "inside Packet::fromString(char *str, char prov)\tPacket.cpp line 62" << std::endl;
		std::cerr << "Try error:\t" << e.what() << std::endl;
	}
}

char *Packet::toCharString(char prov)
{
	std::vector<char> *res = new std::vector<char>();
	res->resize(PACKET_SIZE_PACKET);
	for (int i = 0; i < PACKET_SIZE_PACKET; i++)
		(*res)[i] = 0;
	(*res)[PACKET_INDEX_TYPE] = (char)_type;

	if (_dataSize > PACKET_SIZE_MAX_DATA - PACKET_SIZE_NAME) {
		std::cerr << "inside Packet::toCharString(char prov)\tPacket.cpp line 77\nWARNING : _dataSize is superior to PACKET_SIZE_MAX_DATA, so it will be truncated to it." << std::endl;
		_dataSize = PACKET_SIZE_MAX_DATA - PACKET_SIZE_NAME;
	}
	(*res)[PACKET_INDEX_DATASIZE] = (int)(_dataSize / 1000);
	(*res)[PACKET_INDEX_DATASIZE + 1] = (int)((_dataSize%1000) /  100);
	(*res)[PACKET_INDEX_DATASIZE + 2] = (int)((_dataSize%1000%100) /  10);
	(*res)[PACKET_INDEX_DATASIZE + 3] = (int)((_dataSize%1000%100%10) );

	if (_type == PACKET_USERNAME)
		for (int i = 0; i < PACKET_SIZE_NAME && i < _username.size(); i++)
			(*res)[PACKET_INDEX_DATA + i] = _username[i];
	if (_type == PACKET_SPAWN) {
		for (int i = 0; i < PACKET_SIZE_NAME && i < _entity.name.size(); i++)
			(*res)[PACKET_INDEX_DATA + i] = _entity.name[i];
		fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME, _entity.x);
		fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + (PACKET_SIZE_POS),_entity.y);
		fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 2*(PACKET_SIZE_POS),_entity.health);
		fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 3*(PACKET_SIZE_POS),_entity.score);
		for (int i = 0; i < PACKET_SIZE_NAME && i < _entity.weapon.size(); i++)
			(*res)[PACKET_INDEX_DATA + PACKET_SIZE_NAME + 4*(PACKET_SIZE_POS) + i] = _entity.weapon[i];
	}
	if (_type == PACKET_UPDATE) {
		for (int i = 0; i < _entities.size(); i++) {
			for (int j = 0; j < PACKET_SIZE_NAME && j < _entities[i].name.size(); j++)
				(*res)[PACKET_INDEX_DATA + j + (i * PACKET_SIZE_ENTITY)] = _entities[i].name[j];
			fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + (i * PACKET_SIZE_ENTITY), _entities[i].x);
			fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + PACKET_SIZE_POS + (i * PACKET_SIZE_ENTITY), _entities[i].y);
			fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 2*PACKET_SIZE_POS + (i * PACKET_SIZE_ENTITY), _entities[i].health);
			fourCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + 3*PACKET_SIZE_POS + (i * PACKET_SIZE_ENTITY), _entities[i].score);
			for (int j = 0; j < PACKET_SIZE_NAME && j < _entities[i].weapon.size(); j++)
				(*res)[PACKET_INDEX_DATA+PACKET_SIZE_NAME+4*(PACKET_SIZE_POS)+(i*PACKET_SIZE_ENTITY)+j] = _entities[i].weapon[j];
		}
	}
	if (_type == PACKET_LIST_ROOMS) {
		for (int i = 0; i < _rooms.size(); i++) {
			for (int j = 0; j < PACKET_SIZE_NAME && j < _rooms[i].name.size(); j++)
				(*res)[PACKET_INDEX_DATA + j + (i * PACKET_SIZE_ROOM)] = _rooms[i].name[j];
			eightCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME + (i * PACKET_SIZE_ROOM), _rooms[i].port);
		}
	}
	if (_type == PACKET_JOIN_ROOM) {
		for (int i = 0; i < PACKET_SIZE_NAME && i < _toJoin.name.size(); i++)
			(*res)[PACKET_INDEX_DATA + i] = _toJoin.name[i];
		eightCharFromNumber(res, PACKET_INDEX_DATA + PACKET_SIZE_NAME, _toJoin.port);
	}
	if (_type == PACKET_CREATE_ROOM)
		for (int i = 0; i < PACKET_SIZE_NAME && i < _toCreate.size(); i++)
			(*res)[PACKET_INDEX_DATA + i] = _toCreate[i];
	if (_type == PACKET_CREATED_ROOM)
		eightCharFromNumber(res, PACKET_INDEX_DATA, _createdPort);
	if (_type == PACKET_SHOOT)
		for (int i = 0; i < PACKET_SIZE_NAME && i < _shooter.size(); i++)
			(*res)[PACKET_INDEX_DATA + i] = _shooter[i];
	return res->data();
}

std::string Packet::toString(char prov)
{
	char *str = toCharString(prov);
	std::string res;
	res.resize(PACKET_SIZE_PACKET);
	for (int i = 0; i < PACKET_SIZE_PACKET; i++)
		res[i] = str[i];
	return res;
}

/* UTILITARIES */

void Packet::printPacket()
{
	int segfaultMemory = _dataSize;
	std::cout << std::flush;
	std::cout << "Packet\n";
	std::cout << "\t_type\t\t" << typeToString(_type) << "\n";
	std::cout << "\t_dataSize\t" << _dataSize << "\n";
	if (_dataSize > PACKET_SIZE_MAX_DATA)
		std::cerr << "_dataSize is over PACKET_SIZE_MAX_DATA ! Incoming segfault." << std::endl;
	std::cout << "\t_username\t\t" << _username << "\n";
	std::cout << "\t_toCreate\t\t" << _toCreate << "\n";
	std::cout << "\t_createdPort\t" << _createdPort << "\n";
	displayEntity(_entity);
	displayEntities(_entities);
	displayRoom(_toJoin, true);
	displayRooms(_rooms);
	std::cout << "\t_shooter\t" << _shooter << "\n";
	std::cout << std::flush;
	_dataSize = segfaultMemory;
}

std::string Packet::typeToString(PacketType type)
{
	if (type == PACKET_PING)
		return "PING/PONG";
	if (type == PACKET_USERNAME)
		return "USERNAME";
	if (type == PACKET_GET_ROOMS)
		return "GET_ROOMS";
	if (type == PACKET_LIST_ROOMS)
		return "LIST_ROOMS";
	if (type == PACKET_JOIN_ROOM)
		return "JOIN_ROOM";
	if (type == PACKET_CREATE_ROOM)
		return "CREATE_ROOM";
	if (type == PACKET_CREATED_ROOM)
		return "CREATED_ROOM";
	if (type == PACKET_SPAWN)
		return "SPAWN";
	if (type == PACKET_UPDATE)
		return "UPDATE";
	if (type == PACKET_SHOOT)
		return "SHOOT";
	return "INVALID (plz fix)";
}

std::string Packet::backwardsString(const char *s)
{
	std::string r = "";
	if (!s)
		return r;
	for (int i = 0; i < PACKET_SIZE_NAME && s[i]; i++)
		r += s[i];
	return r;
}

// int HELPERFUNCTION()
// {
// 	/* SEND SPAWN PACKET */
// 	Packet p;
// 	p.setType(PACKET_SPAWN);
// 	p.setEntity(entity("MyName", X, Y));


// 	/* READ SPAWN PACKET */

// 	Packet p;
// 	p.fromString( boost buffer);
// 	p.getEntity().x;
// 	p.getEntity().y;

// 	/* SEND UPDATE PACKET */

// 	Packet p;
// 	names = ["A", "B", "C"];
// 	positions = [ (0,0), (100,100), (500, 500)];
// 	p.setType(PACKET_UPDATE);
// 	for (int i =0; i < 3; i++) {
// 		p.addEntity(	entity(names[i], positions[i][0], positions[i][1])	);
// 	}

// 	/* READ UPDATE PACKET */

// 	Packet p;
// 	p.fromString( boost buffer);
// 	std::vector<entity> entities = p.getEntities();
// 	for (int i = 0; i < entities.size(); i++) {
// 		entities[i].x;
// 		entities[i].name;
// 		entities[i].score;
// 	}
// }
