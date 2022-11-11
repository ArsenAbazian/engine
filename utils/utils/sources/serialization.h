#ifndef A3D_SERIALIZATION_INC
#define A3D_SERIALIZATION_INC

#define a3dVersion(major,minor) ((((long)major) << 16) | ((long)minor))
#define MAKE_HEADER(a, b, c, d) ((((int)d) << 24) | (((int)c) << 16) | (((int)b) << 8) | (a))

class a3dMemoryFile;

class a3dSerializableObject { 
protected:
	int version;
public:
	a3dSerializableObject() { this->version = 0x00010000; }
	~a3dSerializableObject() { }
	
	virtual int getHeaderCode() { return this->version; } 
	virtual int calcSize() { return 4 + 4 + 4 + this->calcSizeCore(); }
	virtual int calcSizeCore() { return 0; }
	virtual bool saveToMemory( a3dMemoryFile &file );
	virtual bool loadFromMemory( a3dMemoryFile &file );
	virtual bool saveToMemoryCore( a3dMemoryFile &file );
	virtual bool loadFromMemoryCore( a3dMemoryFile &file );

	bool supportVersion(int v) { return this->version >= v; }
};

#endif