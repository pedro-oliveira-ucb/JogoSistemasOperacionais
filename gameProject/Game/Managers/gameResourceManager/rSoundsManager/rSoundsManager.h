#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>

#include "../../../gameResources/gameResource/rSound/rSound.h"
#include "../../../gameResources/rBaseResource/rBaseResource.h"


class rSoundsManager : public rBaseResource
{
	std::mutex mtx;

	std::unordered_map<std::string , std::unique_ptr<rSound>> sounds;

public:
	rSoundsManager( std::string path ) :rBaseResource( path ) { }

	bool initialize( ) override;
	void setVolume( float volume );
	bool playSound( std::string soundName );
};

