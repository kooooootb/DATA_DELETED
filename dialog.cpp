#include "dialog.h"
#include "Game.h"
#include "EditTool.h"

namespace Dialog{
	dialog::dialog() {
		std::fstream fs(PRESETS_CFG, std::ios::in);
		if (!fs.is_open()) {
			fs.open(PRESETS_CFG, std::ios::trunc | std::ios::in);
		}
		
		std::string line;
		while(std::getline(fs, line)){
			while(std::all_of(line.begin(), line.end(), isspace)){
				if(std::getline(fs, line).eof()) return;
			}
			presets.push_back(line);
		}
		
		if(presets.empty() || presets.size() % 3 != 0){
			fs.close();
			fs.open(PRESETS_CFG, std::ios::trunc | std::ios::out);
			
			presets.emplace_back(CELLS_CFG);
			presets.emplace_back(ITEMS_CFG);
			presets.emplace_back(CREATURES_CFG);
			fs << CELLS_CFG << std::endl << ITEMS_CFG << std::endl << CREATURES_CFG << std::endl;
		}
		
		cells_cfg = presets[0];
		items_cfg = presets[1];
		creatures_cfg = presets[2];
		
		fs.close();
		
		refreshDialog();
	}
	
	void dialog::refreshDialog() {
		msgs[0] = "1 - Start new game";
		msgs[1] = "2 - Edit map";
		msgs[2] = "3 - Load saved game";
		msgs[3] = "4 - Switch preset\n\t\tCurrent cell cfg file is:" + cells_cfg + "\n\t\tCurrent items cfg file is:" + items_cfg + "\n\t\tCurrent creatures cfg file is:" + creatures_cfg;
		msgs[4] = "5 - Edit current preset";
		msgs[5] = "6 - Add new preset";
		msgs[6] = "7 - Delete new preset";
		msgs[7] = "8 - List presets";
		msgs[8] = "0 - Quit";
	}
	
	void dialog::refreshFile(){
		std::ofstream fs(PRESETS_CFG, std::ios::trunc);
		if (!fs.is_open()) {
			throw std::runtime_error("Can't create files");
		}
		
		for(const auto& it : presets){
			fs << it << std::endl;
		}
		
		fs.close();
	}
	
	void dialog::menu() {
		void (dialog::*fptr[])() = {nullptr, &dialog::startGame, &dialog::editMap, &dialog::loadGame, &dialog::switchPreset, &dialog::editPreset,
									&dialog::addPreset, &dialog::deletePreset, &dialog::listPresets};
		int rc = getOption();
		while(rc){
			void (dialog::*func)() = fptr[rc];
			(this->*func)();
			rc = getOption();
		}
	}
	
	int dialog::getOption() {
		const int Nmsgs = sizeof(msgs) / sizeof(msgs[0]);
		const char *errmsg = "";
		int rc;
		do {
			std::cout << errmsg;
			errmsg = "Error, repeat:\n";
			for (auto & msg : msgs)
				std::cout << msg << std::endl;
			std::cout << "Make your choice: ";
			if(getNum(rc, std::cin) < 0) throw(std::exception());
		} while (rc < 0 || rc >= Nmsgs);
		return rc;
	}
	
	void dialog::startGame(){
		nodata::Game game(cells_cfg, items_cfg, creatures_cfg);
		
		game.start();
	}
	
	void dialog::editMap(){
		edittool::EditTool edit(cells_cfg, items_cfg, creatures_cfg);
		
		edit.start();
	}
	
	void dialog::loadGame(){
		std::string name;
		std::cout << "Input saved game filename:";
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		std::getline(std::cin, name);
		
		try{
			nodata::Game game(name);
			game.start();
		}catch(std::runtime_error &re){
			std::cout << re.what() << std::endl;
		}
	}
	
	void dialog::changeCellsFile() {
		std::cout << "Input new cells cfg filename:";
		std::getline(std::cin, cells_cfg);
		refreshDialog();
	}
	
	void dialog::changeItemsFile() {
		std::cout << "Input new items cfg filename:";
		std::getline(std::cin, items_cfg);
		refreshDialog();
	}
	
	void dialog::changeCreaturesFile() {
		std::cout << "Input new creatures cfg filename:";
		std::getline(std::cin, creatures_cfg);
		refreshDialog();
	}
	
	void dialog::switchPreset() {
		int choice;
		std::cout << "Input preset's index:";
		if(getNum(choice, std::cin) < 0) return;
		
		if(choice >= 1 && choice <= presets.size() / 3){
			curPreset = choice;
			choice -= 1;
			cells_cfg = presets[choice * 3];
			items_cfg = presets[choice * 3 + 1];
			creatures_cfg = presets[choice * 3 + 2];
		}
		
		refreshDialog();
	}
	
	void dialog::editPreset() {
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		changeCellsFile();
		changeItemsFile();
		changeCreaturesFile();
		presets[curPreset] = cells_cfg;
		presets[curPreset + 1] = items_cfg;
		presets[curPreset + 2] = creatures_cfg;
		
		refreshFile();
		refreshDialog();
	}
	
	void dialog::addPreset() {
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		changeCellsFile();
		changeItemsFile();
		changeCreaturesFile();
		presets.emplace_back(cells_cfg);
		presets.emplace_back(items_cfg);
		presets.emplace_back(creatures_cfg);
		
		refreshFile();
		refreshDialog();
	}
	
	void dialog::deletePreset() {
		int choice;
		std::cout << "Input preset's index:";
		if(getNum(choice, std::cin) < 0) return;
		
		if(choice >= 1 && choice <= presets.size() / 3 && presets.size() != 1){
			choice -= 1;
			presets.erase(presets.begin() + choice * 3);
			presets.erase(presets.begin() + choice * 3);
			presets.erase(presets.begin() + choice * 3);
			
			cells_cfg = presets[0];
			items_cfg = presets[1];
			creatures_cfg = presets[2];
		}
		
		refreshFile();
		refreshDialog();
	}
	
	void dialog::listPresets() {
		for(int i = 0;i < presets.size() / 3;++i){
			std::cout << "Preset #" << i + 1 << ':' << std::endl;
			std::cout << "\tcells cfg file:" << presets[i * 3] << std::endl;
			std::cout << "\titems cfg file:" << presets[i * 3 + 1] << std::endl;
			std::cout << "\tcreatures cfg file:" << presets[i * 3 + 2] << std::endl;
		}
	}
}