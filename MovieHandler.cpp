#include <fstream>
#include <vector>
#include "MovieHandler.h"

MovieHandler::MovieHandler() 
{
	srand(0);  
	cache = new Cache;
	head = new Node("null", "null", nullptr, nullptr); 
	readList();
}

void MovieHandler::readList()
{	
	std::ifstream file(FILE_NAME);
	
	if(!file.is_open())
	{
		std::cout << "--- Movie List ----" << std::endl;
		std::cout << "ERROR: There was an issue opening the movies file" << std::endl;
		std::cout << "Please make sure the \"" << FILE_NAME <<  "\" exists!" << std::endl;
		return;
	}

	Node * temp = nullptr;
	std::string line = "null";
	while(getline(file, line)) 
	{
		Node * newNode = new Node(line, getURL(line, false),  nullptr, nullptr);
		if(head->movie != "null")
		{
			temp = head;
			while(temp->next != nullptr)
			{
				
				temp = temp->next;
			}
			
			temp->next = newNode;
			newNode->previous = temp;
		}
		else if(head->movie == "null")
		{
			head = newNode;
		}
	}
}

void MovieHandler::writeList()
{
	std::ofstream file(FILE_NAME);
	if(!file.is_open())
	{
		std::cout << "--- Movie List ---" << std::endl;
		std::cout << "ERROR: There was an issue opening the movies file" << std::endl;
		std::cout << "Please make sure the \"" << FILE_NAME << "\" exists!" << std::endl;
		return;
	}
	
	std::string movie;
	Node * temp = head;
	while(temp != nullptr)
	{
		file << temp->movie << std::endl;
		temp = temp->next;	
	}
	file.close();
	
}

void MovieHandler::print()
{
	Node * print = head;
	int i = 1;
	std::cout << "--- Movie List ---" << std::endl;
	if(head == nullptr)
	{
		std::cout << "There are currently no movies for you to watch!" << std::endl;
		std::cout << "Add some with the add <movie> command!" << std::endl;
		return;
	}
	while(print != nullptr)
	{
		std::cout << i << ".) " << print->movie << std::endl;
		i++;
		print = print->next; 
	}
}

void MovieHandler::addMovie(std::string movie)
{
	Node * newNode = new Node(movie, getURL(movie, false), nullptr, nullptr);
	if(head == nullptr || head->movie == "null")
	{
		head = newNode;
		return;
	}
	Node * placement = head;
	while(placement->next != nullptr)
	{
		placement = placement->next;
	}
	
	placement->next = newNode;
	newNode->previous = placement;

	std::cout << "--- Movie List ---" << std::endl;
	std::cout << "You added the movie: " << newNode->movie << std::endl;
	std::cout << "URL: " << newNode->URL << std::endl;
	std::cout << " ------------" << std::endl;
	
}

void MovieHandler::removeMovie(std::string title)
{
	Node * search = head;
	if(title == head->movie)
	{
		head = search->next;
		search->previous = nullptr;
		delete search;

	        std::cout << "--- Movie List ---" << std::endl;
       		std::cout << "Removing \""<< title << "\" fromyour movie list!" << std::endl;
       	 	std::cout << "The movie you requested has been removed." << std::endl;
		return;
	}

	while(search->movie != title || search == nullptr)
	{	
		search = search->next;
	}

	if(search == nullptr)
	{
		std::cout << "--- Movie List ---" << std::endl;
		std::cout << "Error: The movie \"" << title << "\" was not found in your list!" << std::endl;
		return;
	}
	
	Node * previous = search->previous;
	Node * next = search->next;

	previous->next = next;
	next->previous = previous;
	
	search->previous = nullptr;
	search->next = nullptr;

	delete search;
	
	std::cout << "--- Movie List ---" << std::endl;
	std::cout << "Removing \""<< title << "\" fromyour movie list!" << std::endl;
	std::cout << "The movie you requested has been removed." << std::endl;
}
void MovieHandler::pop()
{
	if(head == nullptr)
	{
		std::cout << "--- Movie List ---" << std::endl;
		std::cout << "Error: There's no more movies in your list to watch!" << std::endl;
		std::cout << "Use \"Add <movie title\" to add more movies!" << std::endl;
		return;
	}
	Node * temp = head->next;
	Node * toDelete = head;
	head = temp;
	std::string name = toDelete->movie;
	delete toDelete;

	std::cout << "--- Movie List ---" << std::endl;
	std::cout << "You watched the movie \"" << name << "\"!" << std::endl;
	if(head != nullptr)
	{
		std::cout << "The next movie in your list is: \"" << head->movie << "\""<< std::endl;
		std::cout << "URL: " << head->URL << std::endl; 
	}
	else
	{
		std::cout << "There are no more movies in your list! Add some with the \"add <movie>\" command!" << std::endl;
	}
	
}
std::string MovieHandler::getURL(std::string name, bool msg)
{
	std::vector<std::string> msgs = 
	{
		"Here you might find this link informative: ", 
		"I got a Rotten Tomatoes link for you: ",
		"Here's some more information about your movie: ",
		"You might find some more information here: "
	};
	
	int r = rand() % 4;
	
	for(unsigned int i = 0; i < name.size(); i++)
		if(name[i] == ' ')
			name[i] = '_';
	std::string URL = URL_BUFFER + name;
	std::string toReturn = (msg) ? msgs[r] + URL : URL;

	return toReturn;
}

void MovieHandler::updateCache(std::string _currentMovie, std::string _URL, int _index)
{
	cache->currentMovie = _currentMovie;
	cache->URL = _URL;
	cache->index = _index;
}

std::string MovieHandler::getCacheAttribute(Attribute attribute)
{
	switch(attribute)
	{
		case MOVIE:
			return cache->currentMovie;		
			break;
		case URL:
			return cache->URL;
			break;
		case INDEX:
			return std::to_string(cache->index);
			break;
	}
	return "null";	
}

MovieHandler::~MovieHandler() { writeList(); };
