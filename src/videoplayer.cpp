#include "videoplayer.h"

#include <iostream>
#include <map>

std::string VideoPlayer::
title_video_id_tags(const std::string&title, const std::vector<std::string>&tags, const std::string&id) {
   std::string answer;
   answer += title;

   answer += " (";
   answer += id;
   answer += ") ";

   answer += "[";

   int size = tags.size();
   int counter = 0;

   while (counter < size) {
       answer += tags[counter];

       if (counter+1!= size) {
           answer += " ";      // this logic is for the space appearing between tags.
       }

       ++counter;
   }
   answer += "]";

   return answer;
}


void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {

    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::map<std::string, std::pair<std::string, std::vector<std::string>>>sorted;   // title, id,tag.

    for (const auto& video : videos) {
        sorted.insert({ video.getTitle(), { video.getVideoId(),video.getTags()} });
    }
    std::string temp;

    std::cout << "Here's a list of all available videos:" << std::endl;

    for (const auto& pair : sorted) {
        temp = title_video_id_tags(pair.first, pair.second.second, pair.second.first);
        std::cout << temp;

        if (flaggedVideos.find(pair.first) != flaggedVideos.end()) {
            std::cout << " - FLAGGED " << "(" << flaggedVideos[pair.first] << ")";
        }
            std::cout << std::endl;

    }

}

void VideoPlayer::playVideo(const std::string& videoId) {

     const auto video = mVideoLibrary.getVideo(videoId);
    if (video == nullptr) {
        std::cout << "Cannot play video: Video does not exist" << std::endl;
    }
    else if (flaggedVideos.find(video->getTitle()) != flaggedVideos.end()) {
        std::cout << "Cannot play video: Video is currently flagged " << "(" << flaggedVideos[video->getTitle()] << ")" << std::endl;
    }

    else if (currentlyPlaying.size() > 0) {
        stopVideo();
        std::cout << "Playing video: " << video->getTitle() << std::endl;
        currentlyPlaying.emplace_back(video);   // insert the video.
    }

    else if (paused.size() > 0) {
       
        stopVideo();
        std::cout << "Playing video: " << video->getTitle() << std::endl;
        currentlyPlaying.emplace_back(video);
    }

    else {
        std::cout << "Playing video: " << video->getTitle() << std::endl;
        currentlyPlaying.emplace_back(video);
    }
}

void VideoPlayer::stopVideo() {
  
    // you can only stop what is playing or paused.

    if (currentlyPlaying.size() > 0) {
        std::cout << "Stopping video: " << currentlyPlaying[0]->getTitle() << std::endl;
        currentlyPlaying.clear();
        return;
    }
    else if (paused.size() > 0) {
        std::cout << "Stopping video: " << paused[0]->getTitle() << std::endl;
        paused.clear();
        return;
    }
    else {
        std::cout << "Cannot stop video: No video is currently playing" << std::endl;
    }
}

void VideoPlayer::playRandomVideo() {
    std::vector<Video> videos = mVideoLibrary.getVideos();
  
    
    int index = rand() % videos.size();   // generate between [ 0,videos.size()  )         // notice that this is a closed-open interval.

    playVideo(videos[index].getVideoId());
}
// :(     seems like there is an error in part1test for this one.... maybe, maybe not

void VideoPlayer::pauseVideo() {

    if (currentlyPlaying.size() > 0) {
        std::cout << "Pausing video: " << currentlyPlaying[0]->getTitle() << std::endl;
        paused.emplace_back(currentlyPlaying[0]);
        currentlyPlaying.clear();
        return;
    }
    else if (paused.size() > 0) {
        std::cout << "Video already paused: " << paused[0]->getTitle() << std::endl; 
    }
    else {
        std::cout << "Cannot pause video: No video is currently playing" << std::endl;
    }
}

void VideoPlayer::continueVideo() {
  
    if (paused.size() > 0) {
        std::cout << "Continuing video: " << paused[0]->getTitle() << std::endl;
        paused.clear();
    }
    else if (currentlyPlaying.size() > 0) {
        std::cout << "Cannot continue video: Video is not paused" << std::endl;
    }
    else {
        std::cout << "Cannot continue video: No video is currently playing" << std::endl;
    } 
}

void VideoPlayer::showPlaying() {
    if (currentlyPlaying.size() > 0) {
        const auto video = currentlyPlaying[0];
        std::string temp=title_video_id_tags(video->getTitle(), video->getTags(), video->getVideoId());
        std::cout << "Currently playing: " << temp << std::endl;
    }
    else if (paused.size() > 0) {
        const auto video = paused[0];
        std::string temp = title_video_id_tags(video->getTitle(), video->getTags(), video->getVideoId());
        std::cout << "Currently playing: " << temp << " - PAUSED" << std::endl;

    }
    else {
        std::cout << "No video is currently playing" << std::endl;
    }
   
}

//------------------------Part2--------------------------------

void VideoPlayer::createPlaylist(const std::string& playlistName) {
    bool checker = mVideoPlayList.isItPresent(playlistName); 

    if (checker) {
        std::cout << "Cannot create playlist: A playlist with the same name already exists" << std::endl;
        return;
    }
    else {
        std::cout << "Successfully created new playlist: " << playlistName << std::endl;
        mVideoPlayList.pushPlayList(playlistName);
    }
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
    bool check = mVideoPlayList.isItPresent(playlistName);
    const auto video = mVideoLibrary.getVideo(videoId);
    if (!check) {
        std::cout << "Cannot add video to " << playlistName << ": Playlist does not exist" << std::endl;
        return;
    }

    else if (video == nullptr) {
        std::cout << "Cannot add video to " << playlistName << ": Video does not exist" << std::endl;
        return;
    }

    else if (flaggedVideos.find(video->getTitle()) != flaggedVideos.end()) {
        std::cout << "Cannot add video to " << playlistName << ": " << "Video is currently flagged " << "(" << flaggedVideos[video->getTitle()];
        std::cout << ")" << std::endl;
        return;
    }

    // at this point, it is assured that the playlist exist.

    std::pair<std::string,std::pair<std::string,std::string> >playlist = mVideoPlayList.aPlayList(playlistName);    // returns the playlist.

    if(playlist.second.second == "") { 
        std::cout << "Added video to " << playlistName << ": " << video->getTitle() << std::endl;
        mVideoPlayList.insert(playlistName,video->getTitle());
        return;
    }

    else if(playlist.second.second==video->getTitle()){
        std::cout << "Cannot add video to " << playlistName << ": " << "Video already added" << std::endl;
    }
}

void VideoPlayer::showAllPlaylists() {

    auto first = mVideoPlayList.playlistBeginiterator();
    auto last = mVideoPlayList.playlistEndIterator();
    if (first == last) {
        std::cout << "No playlists exist yet" << std::endl;
        return;
    }
    std::cout << "Showing all playlists:" << std::endl;
    for (auto i = first; i != last; ++i) {
        std::cout << i->second.first << std::endl;
    }
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {
    bool check = mVideoPlayList.isItPresent(playlistName);
    if (!check) {
        std::cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << std::endl;
        return;
    }

    else {

        auto playlist = mVideoPlayList.aPlayList(playlistName);
        const auto video = mVideoLibrary.getVideo(playlist.second.second);
        std::cout << "Showing playlist: " << playlistName << std::endl;
        if (video==nullptr) {
            std::cout << "No videos here yet" << std::endl;
            return;
        }
        else {
           // if(video!=nullptr)
                std::string temp = title_video_id_tags(video->getTitle(), video->getTags(), video->getVideoId());
            //std::string temp;
                std::cout << temp;
                if (flaggedVideos.find(video->getTitle()) != flaggedVideos.end()) {
                    std::cout << " - FLAGGED " << "(" << flaggedVideos[video->getTitle()] << ")";
                }
                std::cout << std::endl;
        }
    }

}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
    bool check = mVideoPlayList.isItPresent(playlistName);
    const Video* video = mVideoLibrary.getVideo(videoId);

    if (!check) {
        std::cout << "Cannot remove video from " << playlistName <<": Playlist does not exist" << std::endl;
        return;
    }
    else if (video == nullptr) {
        std::cout << "Cannot remove video from " << playlistName << ": Video does not exist" << std::endl;
        return;
    }

    else {
        auto getplaylist = mVideoPlayList.aPlayList(playlistName);
        if (getplaylist.second.second =="") {
            std::cout << "Cannot remove video from " << playlistName << ": Video is not in playlist" << std::endl;
            return;
        }
        else {
            std::cout << "Removed video from " << playlistName << ": " << video->getTitle() << std::endl;
            mVideoPlayList.insert(playlistName,"");
        }
    }

}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {

    bool check = mVideoPlayList.isItPresent(playlistName);
    if (!check) {
        std::cout << "Cannot clear playlist " << playlistName << ": Playlist does not exist" << std::endl;
    }
    else {
       
        auto first = mVideoPlayList.playlistBeginiterator();
        auto last = mVideoPlayList.playlistEndIterator();
        for (auto i = first; i != last; ++i) {
            mVideoPlayList.insert(i->first, "");
        }

        //mVideoPlayList.insert(playlistName, "");
        std::cout << "Successfully removed all videos from " << playlistName << std::endl;
    }
}


void VideoPlayer::deletePlaylist(const std::string& playlistName) {
    bool check = mVideoPlayList.isItPresent(playlistName);
    if (check) {
        mVideoPlayList.removeplaylist(playlistName);
        std::cout << "Deleted playlist: " << playlistName << std::endl;
    }

    else {
        std::cout << "Cannot delete playlist " << playlistName <<": Playlist does not exist" << std::endl;
    }
}




void VideoPlayer::searchVideos(const std::string& searchTerm) {
    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::map<std::string, const Video*>sortedvalues;

    for (const auto video : videos) {
        if (flaggedVideos.find(video.getTitle()) != flaggedVideos.end())continue;

        std::string value = mVideoPlayList.toLowerCase(video.getTitle());
        std::string subvalue = mVideoPlayList.toLowerCase(searchTerm);
        if (value.find(subvalue) != std::string::npos) {
            sortedvalues.insert({value, mVideoLibrary.getVideo(video.getVideoId()) });
        }
    }

    if (sortedvalues.size() == 0) {
        std::cout << "No search results for " << searchTerm << std::endl;
    }
    else {
        std::cout << "Here are the results for " << searchTerm <<":" <<  std::endl;
        int counter = 1;
        std::string temp;
        const Video* video = nullptr;
        for (auto start = sortedvalues.begin(); start != sortedvalues.end(); ++start) {
            video = mVideoLibrary.getVideo(start->second->getVideoId());
            temp = title_video_id_tags(video->getTitle(), video->getTags(), video->getVideoId());
            std::cout << counter << ") " << temp << std::endl;
            ++counter;
        }
        --counter;
        std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << std::endl;
        std::cout << "If your answer is not a valid number, we will assume it's a no." << std::endl;
        int k = 0;
        std::cin >> k;
        if (k > 0 && k <= counter) {
            auto start = sortedvalues.begin();
            --k;
            while (k) {
                ++start;
                --k;
            }
            playVideo(start->second->getVideoId());
        }
    }
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::map<std::string, const Video*>sortedvalues;
    std::string temp;
    if (mVideoPlayList.toLowerCase(videoTag).find("#") == std::string::npos) {
        std::cout << "No search results for " << videoTag << std::endl;
        return;
    }

    for (const auto video : videos) {
        if (flaggedVideos.find(video.getTitle()) != flaggedVideos.end())continue;    // dont add flagged videos in the search
        std::vector<std::string> tags = video.getTags();
                       for (const std::string& value : tags) temp += value;    // concatenate all tags present in the vector<string>tags.

         std::string value = mVideoPlayList.toLowerCase(temp);
         std::string subvalue = mVideoPlayList.toLowerCase(videoTag);

        if (value.find(subvalue) != std::string::npos) {
            sortedvalues.insert({ video.getTitle(), mVideoLibrary.getVideo(video.getVideoId()) });
        }                                 // this part gets the video for a particular id.
        temp = "";
    }

    if (sortedvalues.size() == 0) {
        std::cout << "No search results for " << videoTag << std::endl;
    }

    else {
        std::cout << "Here are the results for " << videoTag << ":" << std::endl;
        int counter = 1;
        std::string temp;
        const Video* video = nullptr;

        for (auto start = sortedvalues.begin(); start != sortedvalues.end(); ++start) {
            video = mVideoLibrary.getVideo(start->second->getVideoId());
            temp = title_video_id_tags(video->getTitle(), video->getTags(), video->getVideoId());
            std::cout << counter << ") " << temp << std::endl;
            ++counter;
        }

        --counter;
        std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << std::endl;
        std::cout << "If your answer is not a valid number, we will assume it's a no." << std::endl;
        int k = 0;
        std::cin >> k;
        if (k > 0 && k <= counter) {
            auto start = sortedvalues.begin();
            --k;
            while (k) {
                ++start;
                --k;
            }
            playVideo(start->second->getVideoId());
        }
    }
}

// part 3 was really nice.
void VideoPlayer::flagVideo(const std::string& videoId) {
 // std::cout << "flagVideo needs implementation" << std::endl;
    std::string reason = "reason: Not supplied";
    const Video* video = mVideoLibrary.getVideo(videoId);

    if (video == nullptr) {
        std::cout << "Cannot flag video: Video does not exist" << std::endl;
    }

    else if (flaggedVideos.find(video->getTitle()) != flaggedVideos.end()) {
        std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    }

    else {
        flaggedVideos.insert({ video->getTitle(), reason });
        if (currentlyPlaying.size() > 0 && flaggedVideos.find(currentlyPlaying[0]->getTitle())!=flaggedVideos.end()) {
            stopVideo();
        }
        else if (paused.size() > 0 && flaggedVideos.find(paused[0]->getTitle()) != flaggedVideos.end()) {
            stopVideo();
        }
        std::cout << "Successfully flagged video: " << video->getTitle() << " (" << reason << ")" << std::endl;
    }
   

}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& r) {
   
    const Video* video = mVideoLibrary.getVideo(videoId);
    std::string reason;
    reason += "reason: ";
    reason += r;
    if (video == nullptr) {
        std::cout << "Cannot flag video: Video does not exist" << std::endl;
    }

    else if (flaggedVideos.find(video->getTitle()) != flaggedVideos.end()) {
        std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    }

    else {

        flaggedVideos.insert({ video->getTitle(), reason });
        if (currentlyPlaying.size() > 0 && flaggedVideos.find(currentlyPlaying[0]->getTitle()) != flaggedVideos.end()) {
            stopVideo();
        }
        else if (paused.size() > 0 && flaggedVideos.find(paused[0]->getTitle()) != flaggedVideos.end()) {
            stopVideo();
        }
        std::cout << "Successfully flagged video: " << video->getTitle() << " (" << reason << ")" << std::endl;
    }

}


void VideoPlayer::allowVideo(const std::string& videoId) {
    const Video* video = mVideoLibrary.getVideo(videoId);
    if (video == nullptr) {
        std::cout << "Cannot remove flag from video: Video does not exist" << std::endl;
    }
    else if (flaggedVideos.find(mVideoLibrary.getVideo(videoId)->getTitle()) == flaggedVideos.end()) {
        std::cout << "Cannot remove flag from video: Video is not flagged" << std::endl;
    }
    else {
        std::cout << "Successfully removed flag from video: " << video->getTitle() << std::endl;
        flaggedVideos.erase(video->getTitle());
    }
}
