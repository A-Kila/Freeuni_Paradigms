using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "imdb.h"

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

// key for bsearch function on actorFile
// pair would require "const"-s which would make the code uglier
struct ActorKey {
  const string player;
  const void *base;
};

// compare function for actors
int cmprActors(const void *first, const void* second) {
  ActorKey key  = *(ActorKey *)first;
  const string firstPl = key.player;
 
  const void *base = key.base;
  int bytesToActor = *(int *)second;
  const char *secondPl = (char *)base + bytesToActor;

  return strcmp(firstPl.c_str(), secondPl);
}

bool imdb::getCredits(const string& player, vector<film>& films) const {
  ActorKey key = {player, actorFile};
  int actorSize = *(int *)actorFile;
  
  void *bytesToPlayer = bsearch(&key, (void*)((int *)actorFile + 1), actorSize, sizeof(int), cmprActors);
  if (bytesToPlayer == NULL) return false;
  void *playerLoc = (char *)actorFile + *(int *)bytesToPlayer;
  
  short *filmSize = (short *)((char *)playerLoc + player.size() + 1);
  if (player.size() % 2 == 0) filmSize = (short *)((char *)filmSize + 1);
  int *filmLoc = (int *)(filmSize + 1);
  if (player.size() % 4 == 2 || player.size() % 4 == 3) filmLoc = (int *)((short *)filmLoc + 1);

  for (int i = 0; i < *filmSize; i++) {
    char *movieLoc = (char *)movieFile + *(filmLoc + i);
    film movie = { movieLoc, 1900 + (int)(*(movieLoc + strlen(movieLoc) + 1)) };
    films.push_back(movie);
  }
  
  return true;
}

// key for bsearch function on MovieFile
struct MovieKey {
  const film &movie;
  const void* base;
};

// compare function for movies
int cmprMovies(const void *first, const void *second) {
  MovieKey key = *(MovieKey *)first;
  film firstFilm = key.movie;

  const void* base = key.base;
  int bytesToMovie = *(int *)second;
  char *title = (char *)base + bytesToMovie;
  film secondFilm = {
    title, 
    1900 + (int)(*(title + strlen(title) + 1))
  };

  if (firstFilm < secondFilm) return -1;
  return !(firstFilm == secondFilm);
}


bool imdb::getCast(const film& movie, vector<string>& players) const {
  MovieKey key = { movie, movieFile }; 
  int moviesSize = *(int *)movieFile;

  void *bytesToMovie = bsearch(&key, (void *)((int *)movieFile + 1), moviesSize, sizeof(int), cmprMovies);
  if (bytesToMovie == NULL) return false;
  void *movieLoc = (char *)movieFile + *(int *)bytesToMovie;

  short *castSize = (short *)((char *)movieLoc + movie.title.size() + 2); // +2-> 1 byte is \0, another is the year
  if (movie.title.size() % 2 != 0) castSize = (short *)((char *)castSize + 1);
  int *actorLoc = (int *)(castSize + 1);
  if (movie.title.size() % 4 == 1 || movie.title.size() % 4 == 2)
    actorLoc = (int *)((short *)actorLoc + 1);

  for (int i = 0; i < *castSize; i++)
    players.push_back((char *)actorFile + *(actorLoc + i));

  return false; 
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
