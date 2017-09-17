#include <bits/stdc++.h>

using namespace std;

typedef pair<ifstream*, vector<string> > mypair;
typedef map<ifstream*, queue<vector<string>>> mymap;

bool sorttype;
vector<int> orderCols;

void readMetadata(vector<pair<string, int> > &cols, int &rowsize) {
	ifstream fp;
	fp.open("Metadata.txt");
	string output;
	while(!fp.eof()) {
		fp >> output;
		char* toks = strtok(&output[0], ",");
		string colname;
		int sizeCol;
		bool flag = false;
		while(toks != NULL) {
			if(!flag) {
				colname = toks;
				flag = true;
			}
			else {
				sizeCol = stoi(toks);
			}
			toks = strtok(NULL, ",");
		}
		rowsize += sizeCol;
		cols.push_back(make_pair(colname, sizeCol));
	}
	fp.close();
	rowsize += cols.size()*2-2;
}

void getColOrder(char* args[], vector<pair<string, int> > &metdata, int size) {
	for(int i = 0; i < size; i++) {
		string temp(args[i]);
		for(int j = 0; j < metdata.size(); j++) {
			if(temp.compare(metdata[j].first) == 0) {
				orderCols.push_back(j);
				break;
			}
		}
	}
}

void formatToVector(vector<vector<string>> &data, string mystr, vector<pair<string, int>> &coldata) {
	vector<string> temp;
	int base = 0;
	for(int i = 0; i < coldata.size(); i++) {
		temp.push_back(mystr.substr(base, coldata[i].second));
		base += coldata[i].second+2;
	}
	data.push_back(temp);
}

void formatToQueue(queue<vector<string>> &data, string mystr, vector<pair<string, int>> &coldata) {
	vector<string> temp;
	int base = 0;
	for(int i = 0; i < coldata.size(); i++) {
		temp.push_back(mystr.substr(base, coldata[i].second));
		base += coldata[i].second+2;
	}
	data.push(temp);
}

bool myfunc1(const vector<string> &a, const vector<string> &b) {
	for(int i = 0; i < orderCols.size(); i++) {
		if(a[orderCols[i]].compare(b[orderCols[i]]) == 0)
			continue;
		else if(a[orderCols[i]].compare(b[orderCols[i]]) < 0)
			return true;
		else
			return false;
	}
	return false;
}

bool myfunc2(const vector<string> &a, const vector<string> &b) {
	for(int i = 0; i < orderCols.size(); i++) {
		if(a[orderCols[i]].compare(b[orderCols[i]]) == 0)
			continue;
		else if(a[orderCols[i]].compare(b[orderCols[i]]) < 0)
			return false;
		else
			return true;
	}
	return false;
}

bool myqueuecomp(mypair &a, mypair &b) {
	if(!sorttype) {
		for(int i = 0; i < orderCols.size(); i++) {
			if((a.second)[orderCols[i]].compare((b.second)[orderCols[i]]) == 0)
				continue;
			else if ((a.second)[orderCols[i]].compare((b.second)[orderCols[i]]) < 0)
				return false;
			else
				return true;
		}
		return false;
	}
	else {
		for(int i = 0; i < orderCols.size(); i++) {
			if((a.second)[orderCols[i]].compare((b.second)[orderCols[i]]) == 0)
				continue;
			else if ((a.second)[orderCols[i]].compare((b.second)[orderCols[i]]) < 0)
				return true;
			else
				return false;
		}	
		return false;
	}
}

int main(int argc, char* argv[]) {
	vector<pair<string, int> > cols;
	int rowsize=0;
	readMetadata(cols, rowsize);
	getColOrder(&argv[5], cols, argc-5);
	ifstream infp;
	infp.open(argv[1], ifstream::in|ifstream::binary|ifstream::ate);
	long long int ipfsize = infp.tellg();
	ipfsize --;
	infp.seekg(0, ios::beg);
	ofstream opfp;
	opfp.open(argv[2], ofstream::binary);
	long long  maxsize = stoi(argv[3])*1024l*512;
	locale loc;
	string a(argv[4]);
	transform(a.begin(), a.end(), a.begin(), ::tolower);
	if(a.compare("asc") == 0) {
		sorttype = false;
	}
	else if (a.compare("desc") == 0) {
		sorttype = true;
	}
	else {
		cout << "choose either desc/asc\n";
		exit(0);
	}
	vector<vector<string> > data;
	string datastr = "";
	string temp;
	int counter = 0;
	while(getline(infp, temp)) {
		formatToVector(data, temp, cols);
		if((data.size()+1)*rowsize >= maxsize) {
			if(!sorttype)
				sort(data.begin(), data.end(), myfunc1);
			else
				sort(data.begin(), data.end(), myfunc2);
			ofstream file1;
			file1.open(to_string(counter) + ".txt", ofstream::binary|ofstream::out);
			for(int i = 0; i < data.size(); i++) {
				for(int j = 0; j < data[i].size(); j++) {
					if(j == data[i].size()-1)
						file1 << data[i][j] << endl;
					else
						file1 << data[i][j] << "  ";
				}
			}
			file1.close();
			counter++;
			data.clear();
		}
	}
	//cout << data.size() << endl;
	if(data.size() > 0) {
		if(!sorttype)
			sort(data.begin(), data.end(), myfunc1);
		else
			sort(data.begin(), data.end(), myfunc2);
		ofstream file1;
		file1.open(to_string(counter) + ".txt", ofstream::binary|ofstream::out);
		for(int i = 0; i < data.size(); i++) {
			for(int j = 0; j < data[i].size(); j++) {
				if(j == data[i].size()-1)
					file1 << data[i][j] << endl;
				else
					file1 << data[i][j] << "  ";
			}
		}
		file1.close();
		counter++;
		data.clear();
	}
	//cout << data.size() << endl;
	vector<vector<string>>().swap(data); 	//free all memory till now
	
	long long sizequeue = (maxsize*1.9)/(rowsize*(counter+2));
	//cout << sizequeue << endl;
	queue<vector<string>> buffer;
	priority_queue< mypair, vector< mypair >, bool (*)(mypair&, mypair&) > myqueue(myqueuecomp);
	mymap mp;
	for(int i = 0; i < counter; i++) {
		ifstream* tempfile = new ifstream();
		tempfile->open(to_string(i) + ".txt", ifstream::in|ifstream::binary);
		string mystr;
		queue<vector<string>> tempqueue;
		while(getline(*tempfile, mystr)) {
			formatToQueue(tempqueue, mystr, cols);
			if(tempqueue.size() >= sizequeue)
				break;
		}
		myqueue.push(make_pair(tempfile, tempqueue.front()));
		tempqueue.pop();
		mp.insert(make_pair(tempfile, tempqueue));
	}
	//cout << myqueue.size() << endl;
	while(!myqueue.empty()) {
		mypair topel = myqueue.top();
		myqueue.pop();
		buffer.push(topel.second);
		if(buffer.size() >= sizequeue) {
			while(!buffer.empty()) {
				for(int i = 0; i < buffer.front().size()-1; i++) {
					opfp << buffer.front()[i] << "  ";
				}
				opfp << buffer.front()[buffer.front().size()-1] << "\n";
				buffer.pop();
			}
		}
		mymap::iterator it = mp.find(topel.first);
		if(it->second.size() > 0) {
			//cout << "second it size " << it->second.size() << endl;
			topel.second = (it->second).front();
			(it->second).pop();
			myqueue.push(topel);
		}
		else {
			string mystr;
			queue<vector<string>> tempqueue;
			while(getline(*(topel.first), mystr)) {
				formatToQueue(tempqueue, mystr, cols);
				if(tempqueue.size() >= sizequeue)
					break;
			}
			if(tempqueue.size() == 0) {
				(it->first)->close();
				mp.erase(it);
			}
			else {
				topel.second = tempqueue.front();
				tempqueue.pop();
				it->second = tempqueue;
				myqueue.push(topel);
			}
		}
	}
	while(!buffer.empty()) {
		for(int i = 0; i < buffer.front().size()-1; i++) {
			opfp << buffer.front()[i] << "  ";
		}
		opfp << buffer.front()[buffer.front().size()-1] << "\n";
		buffer.pop();
	}
	for(int i = 0; i < counter; i++) {
		string filname = to_string(i) + ".txt";
		remove(&filname[0]);
	}
	return 0;
}