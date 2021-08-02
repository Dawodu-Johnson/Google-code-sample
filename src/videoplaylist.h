#pragma once
#include "video.h"
#include<map>
#include<string>
/** A class used to represent a Playlist */

class VideoPlaylist {
private:
	std::map<std::string, std::pair<std::string, std::string> > playlist; // playlistinlowercase, <playlistinoriginalform,id>
public:
	VideoPlaylist() {}
	bool isItPresent(const std::string& s);
	auto playlistBeginiterator() { return playlist.begin(); }
	auto playlistEndIterator(){ return playlist.end(); }
	std::string toLowerCase(const std::string&);
	void pushPlayList(const std::string& s);
	void removeplaylist(const std::string& s) {
		// before calling this function, it is assured that isitpresent() would have been called.
		playlist.erase(toLowerCase(s));
	}
	void insert(const std::string& playlistName,const std::string val) {
		playlist[toLowerCase(playlistName)].second = val;
	
	}
	std::pair<std::string, std::pair<std::string, std::string >>aPlayList(const std::string& playlist);
};
