/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** Packet
*/

#ifndef PACKET_HPP_
#define PACKET_HPP_

#include <string>
#include <vector>
#include <iostream>

enum PacketType {		PACKET_PING=0,
						PACKET_USERNAME=1,
						PACKET_GET_ROOMS=2,
						PACKET_LIST_ROOMS=3,
						PACKET_JOIN_ROOM=4,
						PACKET_CREATE_ROOM=5,
						PACKET_CREATED_ROOM=6,
						PACKET_SPAWN=16,
						PACKET_UPDATE=17,
						PACKET_SHOOT=18				};

enum PacketIndex {		PACKET_INDEX_TYPE=0,
						PACKET_INDEX_DATA=1,
						PACKET_INDEX_DATASIZE=513		};

enum PacketProvenance {	PACKET_PROVENANCE_SERVER=0,
						PACKET_PROVENANCE_CLIENT=1	};

enum PacketSize {		PACKET_SIZE_POS=4,
						PACKET_SIZE_PORT=8,
						PACKET_SIZE_NAME=16,
						PACKET_SIZE_ROOM=24,
						PACKET_SIZE_ENTITY=48,
						PACKET_SIZE_MAX_DATA=512,
						PACKET_SIZE_PACKET=517		};

/*

type
			0x00	PING/PONG		Empty default packet, servers send pong back when pinged by client
			0x01	USERNAME		Username packet, to tell server to link your username and ip
			0x02	GET_ROOMS		Empty packet, to tell server to send room list
			0x03	LIST_ROOMS		List packet with list of rooms (room_name + port)
			0x04	JOIN_ROOM		Join packet, with name and port of room to join
			0x05	CREATE_ROOM		Create packet, with name to tell server to create a new room
			0x06	CREATED_ROOM	Created packet, with port of room that has been created


			0x10	SPAWN		Spawn packet, to tell client an entity must be spawned
			0x11	UPDATE		Update packet, to tell client an entity has updated
			0x12	BULLET		Bullet packet, to tell server client has shot

data
			char[512]			Data to be transmitted, depending on the type

	possible data depending on type
		0x00	PING/PONG	All 512 bytes are filled with zeroes.
		0x01	USERNAME	the first 16 bytes are reserved for the username to set your ip to
		0x02	GET_ROOMS	All 512 bytes are filled with zeroes.
		0x03	LIST_ROOMS	for the number of rooms precized in dataSize, these bits will be filled to describe rooms (name + port)
								the first 16 bytes are reserved for the name of the room
								the next 8 bytes are reserved for the port of the room
		0x04	JOIN_ROOM	the first 16 bytes are reserved for the name of the room to join
							the next 8 bytes are reserved for the port of the room to join
		0x05	CREATE_ROOM the first 16 bytes are reserved for the name of the room to create
		0x06	CREATED_ROOMthe first 8 bytes are reserved for the port of the room that has been created


		0x10	SPAWN		the first 16 bytes are reserved for the name of the entity to spawn
							the next 8 bytes are reserved for the position of the entity
							the next 4 bytes are reserved for the health of the entity
							the next 4 bytes are reserved for the score of the entity
							the next 16 bytes are reserved for the weapon name of the entity
		0x11	UPDATE		for the number of members precised in dataSize, these bits will be filled to describe entities
								the first 16 bytes are reserved for the name of the entity
								the next 8 bytes are reserved for the position of the entity
								the next 4 bytes are reserved for the health
								the next 4 bytes are reserved for the score
								the next 16 bytes are reserved for the weapon name of the entity
		0x12	BULLET		the first 16 bytes are reserved for the name of the entity to spawn


dataSize	char[4]					Size of the actual data needed in the data member

[	type	data	dataSize	]

*/

class entity {
	public:
		entity(const std::string &n="", int X=0, int Y=0){name=n;weapon="", x=X;y=Y;health=0;score=0;};
		~entity(){};
		void sName(const std::string &n="") {name=n;}
		void sWeapon(const std::string &w="") {weapon=w;}
		void sPos(int X=0, int Y=0){ x=X; y=Y;};
		void sHp(int HP=0) { health = HP;};
		void sScore(int Score=0) { score = Score;};
		std::string name;
		std::string weapon;
		int x;
		int y;
		int health;
		int score;
};

class room {
	public:
		room(const std::string &n="", int Port=0) {name=n; port=Port;};
		~room(){};
		std::string name;
		int port;
};

class Packet {
	public:
		Packet(); /* GENERATES A DEFAULT PING PACKET */
		~Packet();

	/* SOCKET UTILITARIES */
		void fromCharString(const char *str, char prov=PACKET_PROVENANCE_CLIENT); /* FILL CLASS DATA FROM STRING */
		void fromString(const std::string &str, char prov=PACKET_PROVENANCE_CLIENT);
		char *toCharString(char prov=PACKET_PROVENANCE_CLIENT); /* GENERATE A STRING FROM CLASS DATA */
		std::string toString(char prov=PACKET_PROVENANCE_CLIENT); /* GENERATE A STRING FROM CLASS DATA */

	/* GETTERS AND SETTERS */
		char getType() { return _type; };
		void setType(char type) { _type = (PacketType)type; };
		void setType(PacketType type) { _type = type; };
		int getDataSize() { return _dataSize; };
		void setDataSize(int size) { _dataSize = size; };
		std::string getUsername() { return _username; };
		void setUsername(const std::string &username);
		entity getEntity() { return _entity; };
		void setEntity(const entity &entity);
		std::vector<entity> getEntities() { return _entities; };
		void addEntity(const entity &entity) { _entities.push_back(entity); };
		room getRoomToJoin() { return _toJoin; };
		void setRoomToJoin(const room &room);
		std::vector<room> getRooms() { return _rooms; };
		void addRoom(const room &room) { _rooms.push_back(room); };
		int getCreatedPort() { return _createdPort; };
		void setCreatedPort(int port) { _createdPort = port; };
		std::string getToCreate() { return _toCreate; };
		void setToCreate(const std::string &ToCreate);
		std::string getShooter() { return _shooter; };
		void setShooter(const std::string &Shooter) { _shooter = Shooter ; };

	/* UTILITARIES */
		void printPacket();
		std::string typeToString(PacketType);
		std::string backwardsString(const char *s);
	private:
		PacketType _type;
		int _dataSize;
		std::string _username;
		entity _entity;
		std::vector<entity> _entities;
		std::string _toCreate;
		int _createdPort;
		room _toJoin;
		std::vector<room> _rooms;
		std::string _shooter;
};

void displayEntity(const entity &e);
void displayEntities(const std::vector<entity> &t);
void displayRoom(const room &e, bool toJoin=false);
void displayRooms(const std::vector<room> &t);

#endif /* !PACKET_HPP_ */
