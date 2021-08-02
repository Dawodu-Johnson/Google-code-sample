#include "videoplaylist.h"

std::string VideoPlaylist:: toLowerCase(const std::string& value) {
    std::string temp;
    for (const auto& val : value) {
        if (isalpha(val)) {
            temp += tolower(val);
        }
        else temp += val;
    }
    return temp;
}

bool VideoPlaylist::isItPresent(const std::string& s) {
    if (playlist.find(toLowerCase(s)) == playlist.end())return false;
    return true;
}

void VideoPlaylist::pushPlayList(const std::string& s) {
   
    playlist.insert({toLowerCase(s),{s,""} });
    // store the lower case for easier comparison, 
    //then store the original version of the string
}

 std::pair<std::string,std::pair<std::string,std::string >> VideoPlaylist::aPlayList(const std::string& playlistname) {
    // this function will only be called when isitpresent(playlistname) has been verified to be true;

    auto answer= playlist.find(toLowerCase(playlistname));
    return *answer;
}