#include "splashkit.h"

using namespace std;

const int MAX_SONGS = 10;

enum Song_Genre_Enum{
	Pop,
	Rock,
	RnB
};

typedef enum Song_Genre_Enum SongGenre;

struct Song_Struct {
    string FileName;
	string Name;
	SongGenre Genre;
	int NoListens;
};

typedef struct Song_Struct Song;

struct Song_List_Struct{
    Song Songs[MAX_SONGS];
    int size;
};

typedef struct Song_List_Struct Song_List;

struct Dotifin_Struct{
    Song_List Songs;
    int TotalListens;
};

typedef struct Dotifin_Struct Dotifin;

int read_integer(string prompt)
{
  string buffer;

  while (1)
  {
    write(prompt);
    buffer = read_line();
    if ( is_integer(buffer) )
      break;
    write_line("Please enter a valid whole number.");
  }
  return convert_to_integer(buffer);
}

int read_integer_range(string prompt, int min, int max)
{
    int inputInt = 0;
    while (1)
    {
        inputInt = read_integer(prompt);
        if(inputInt < min || inputInt > max){
            write_line("Please ensure your input is in the range " + to_string(min) + " , " + to_string(max));
        }
        else{
            break;
        }
    }
  return inputInt;
}

string GenreToString(SongGenre Genre){
    string OutputString = "";
    switch(Genre){
        case(Pop):
        OutputString = "Pop";
        break;
        case(Rock):
        OutputString = "Rock";
        break;
        case(RnB):
        OutputString = "R&B";
        break;
    }
    return OutputString;
}

Song NewSong(){
    Song NewSong = {"", "", SongGenre::Pop, 0};
    write_line("Enter File Name including Extension");
    NewSong.FileName = read_line();
    write_line("Enter Name for Song");
    NewSong.Name = read_line();
    NewSong.Genre = SongGenre(read_integer_range("Enter Genre for Song: Pop(0), Rock(1), RnB(2)", 0, 2));
    return NewSong;
}

music LoadMusic(){
    music MusicToReturn;
    bool bMusicValid = false;
    string FileName = "";
    while(!bMusicValid){
        write_line("Please input the name of the file including the extension:");
        FileName = read_line();
        MusicToReturn = load_music("temp", FileName);
        if(!music_valid(MusicToReturn)){
            write_line("Please make sure your file name is valid.");
        }
        else{
            bMusicValid = true;
        }
    }

    write_line("Please input a name for your sound clip:");
    string SoundName = read_line();
    MusicToReturn = load_music(SoundName, FileName);

    return MusicToReturn;
}

void AddSong(Dotifin& DotifinInstance){
    if(DotifinInstance.Songs.size >= MAX_SONGS){
        return;
    }

    DotifinInstance.Songs.Songs[DotifinInstance.Songs.size] = NewSong();
    DotifinInstance.Songs.size++;
}

void RemoveSong(Dotifin &DotifinInstance, int indexToRemove){
    DotifinInstance.Songs.Songs[indexToRemove] = DotifinInstance.Songs.Songs[DotifinInstance.Songs.size - 1];
    DotifinInstance.Songs.size--;
}

void PlayMusic(music MusicToPlay){
    write_line("Now playing music.");
    play_music(MusicToPlay);
}

void PlaySong(Song& SongToPlay, Dotifin& DotifinInstance){
    PlayMusic(load_music(SongToPlay.Name, SongToPlay.FileName));
    SongToPlay.NoListens++;
    DotifinInstance.TotalListens++;
}

void print_song_details(Song song){
    write_line("Song File Name: " + song.FileName);
    write_line("Song Name: " + song.Name);
    write_line("Song Genre: " + GenreToString(song.Genre));
    write_line("Song Listens: " + to_string(song.NoListens));
}

void AlterSong(Song &SongToAlter){
    write_line("Current Song Details: ");
    print_song_details(SongToAlter);
    write_line("Please input a new Song File Name:");
    SongToAlter.FileName = read_line();
    write_line("Please input a new Song Name:");
    SongToAlter.Name = read_line();
    SongToAlter.Genre = SongGenre(read_integer_range("Please input a new Song Genre: Pop(0), Rock(1), R&B(2)", 0, 2));
    SongToAlter.NoListens = 0;
    write_line("Updated Product Details: ");
    print_song_details(SongToAlter);
}

void view_songs(Dotifin& DotifinInstance){
    if(DotifinInstance.Songs.size == 0){
        write_line("No songs currently! Try adding some!");
        return;
    }
    for(int i = 0; i < DotifinInstance.Songs.size; i++){
        write_line("(" + to_string(i + 1) + ") " + DotifinInstance.Songs.Songs[i].Name);
    }
    int chosenSong = read_integer_range("Please select a song from the above list:", 1, DotifinInstance.Songs.size) - 1;
    write_line("What would you like to do with the chosen song?");
    write_line("(1) - View Details");
    write_line("(2) - Alter Song");
    write_line("(3) - Play Song");
    write_line("(4) - Delete Song");
    int chosenOption = read_integer_range("What would you like to do with the chosen song?", 1, 4);

    switch(chosenOption){
        case 1:
        print_song_details(DotifinInstance.Songs.Songs[chosenSong]);
        break;
        case 2:
        AlterSong(DotifinInstance.Songs.Songs[chosenSong]);
        break;
        case 3:
        PlaySong(DotifinInstance.Songs.Songs[chosenSong],DotifinInstance);
        break;
        case 4:
        RemoveSong(DotifinInstance,chosenSong);
        write_line("The chosen song has been removed.");
        break;
    }
}

void ProcessIntInput(Dotifin& DotifinInstance, int InputInt){
    if(InputInt == 1){
        music LoadedMusic = LoadMusic();
        PlayMusic(LoadedMusic);
    }
    else if(InputInt == 2){
        stop_music();
    }
    else if(InputInt == 3){
        AddSong(DotifinInstance);
    }
    else if(InputInt == 4){
        view_songs(DotifinInstance);
    }
    else if(InputInt == 5){
        write_line("Main Menu!");
    }
}

void print_menu(){
    write_line("Screen Selection:");
    write_line("(1) - Load & Play Music");
    write_line("(2) - Stop Music");
    write_line("(3) - New Song");
    write_line("(4) - View Songs");
    write_line("(5) - Main Menu");
}

int main(){

    write_line("Welcome to Dotifin!");
    Dotifin DotifinInstance;
    DotifinInstance.Songs.size = 0;
    while(!quit_requested()){
        int UserInput;
        print_menu();
        UserInput = read_integer_range("Please select a screen.",1,5);
        ProcessIntInput(DotifinInstance, UserInput);
    }
}