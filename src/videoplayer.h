#pragma once

#include <string>
#include<unordered_map>
#include "videolibrary.h"
#include "videoplaylist.h"

/**
 * A class used to represent a Video Player.
 */
class VideoPlayer {
 private:
  VideoLibrary mVideoLibrary;
  VideoPlaylist mVideoPlayList;
  std::unordered_map<std::string, std::string>flaggedVideos;

 public:
  VideoPlayer() = default;
  std::vector<const Video*>currentlyPlaying; 
  std::vector<const Video*>paused;

  // This class is not copyable to avoid expensive copies.
  VideoPlayer(const VideoPlayer&) = delete;
  VideoPlayer& operator=(const VideoPlayer&) = delete;
  std::string title_video_id_tags(const std::string&, const std::vector<std::string>&, const std::string&);    // formatting.
  // This class is movable.
  VideoPlayer(VideoPlayer&&) = default;
  VideoPlayer& operator=(VideoPlayer&&) = default;

  void numberOfVideos();
  void showAllVideos();
  void playVideo(const std::string& videoId);
  void stopVideo();
  void playRandomVideo();
  void pauseVideo();
  void continueVideo();
  void showPlaying();
  void createPlaylist(const std::string& playlistName);
  void addVideoToPlaylist(const std::string& playlistName, const std::string& videoId);
  void showAllPlaylists();
  void showPlaylist(const std::string& playlistName);
  void removeFromPlaylist(const std::string& playlistName, const std::string& videoId);
  void clearPlaylist(const std::string& playlistName);
  void deletePlaylist(const std::string& playlistName);
  void searchVideos(const std::string& searchTerm);
  void searchVideosWithTag(const std::string& videoTag);
  void flagVideo(const std::string& videoId);
  void flagVideo(const std::string& videoId, const std::string& reason);
  void allowVideo(const std::string& videoId);
};
