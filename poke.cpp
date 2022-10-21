//5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <getopt.h>

using namespace std;

bool mst = false;
bool fasttsp = false;
bool opttst = false;
string a_b_c;

class A_PokeMap {
    private:
        struct Pokemon {
            double x_cord;
            double y_cord;
            char terrain;
        };

        struct PrimNode {
            bool visited = false;
            double dist;
            size_t pred;

            PrimNode(bool visited_in, double dist_in, size_t pred_in) : 
                    visited(visited_in), dist(dist_in), pred(pred_in) {}
        };

        uint32_t num_pokemon;
        double total_dist = 0;
        vector<Pokemon> poke_vec;
        vector<PrimNode> A_prim_vec;

    public:
    void A_read_input() {
        cin >> num_pokemon;
        poke_vec.reserve(num_pokemon);
        int x_cord;
        int y_cord;
        while (cin >> x_cord >> y_cord) {
            Pokemon p;
            p.x_cord = x_cord;
            p.y_cord = y_cord;
            if (x_cord == 0 && y_cord <= 0) {
                //coast
                p.terrain = 'c';
            }
            else if (y_cord == 0 && x_cord <= 0) {
                p.terrain = 'c';
            }
            else if (x_cord < 0 && y_cord < 0) {
                //water
                p.terrain = 'w';
            }
            else {
                //land
                p.terrain = 'l';
            }
            poke_vec.push_back(p);
        }
    }

    double A_dist_helper(Pokemon &p1, Pokemon &p2) {
        if (p1.terrain == p2.terrain || p1.terrain == 'c' || p2.terrain == 'c') {
            double x_dist_sqrd = (p2.x_cord - p1.x_cord) * (p2.x_cord - p1.x_cord);
            double y_dist_sqrd = (p2.y_cord - p1.y_cord) * (p2.y_cord - p1.y_cord);
            return sqrt(x_dist_sqrd + y_dist_sqrd);
        }
        else {
            return numeric_limits<double>::infinity();
        }
    }

    void A_construct_prim() {
        //Initializing prim
        A_prim_vec.reserve(num_pokemon);
        PrimNode pn(false, numeric_limits<double>::infinity(), num_pokemon);
        for (size_t i = 0; i < A_prim_vec.capacity(); ++i) {
            A_prim_vec.push_back(pn);
        }
        //Test--can I make this true and loop thru one less time?
        PrimNode prim_root(false, 0, num_pokemon);
        A_prim_vec[0] = prim_root;

        size_t index = 0;
        size_t true_count = 0;
        for (size_t i = 0; i < num_pokemon; ++i) {
            //STEP 1
            double min_dist = numeric_limits<double>::infinity();
            for (size_t j = 0; j < num_pokemon; ++j) {
                if (A_prim_vec[j].visited == false) {
                    if (A_prim_vec[j].dist < min_dist) {
                        min_dist = A_prim_vec[j].dist;
                        index = j;
                    }
                } 
            }
            //STEP 2
            A_prim_vec[index].visited = true;
            total_dist += A_prim_vec[index].dist;
            ++true_count;
            //STEP 3
            for (size_t j = 0; j < num_pokemon; ++j) {
                if (A_prim_vec[j].visited == false) {
                    double new_dist = A_dist_helper(poke_vec[index], poke_vec[j]);
                    if (new_dist < A_prim_vec[j].dist) {
                        if (j != index) {
                            A_prim_vec[j].dist = new_dist;
                            A_prim_vec[j].pred = index;
                        }
                    }
                }
            }
        }
        //TODO: Is there a better way to error check
        //(i.e. if there are pokemon on both the land and in the sea, with no pokemon on the coastline), 
        if (true_count != num_pokemon) {
            cerr << "Cannot construct MST\n";
            exit(1);
        }
    }

    void part_A_print() {
        cout << total_dist << '\n';
        for (size_t i = 1; i < num_pokemon; ++i) {
            if (i < A_prim_vec[i].pred) {
                cout << i << ' ' << A_prim_vec[i].pred << '\n';
            }
            else {
                cout << A_prim_vec[i].pred << ' ' << i << '\n';
            }
        }
    }

};

class B_PokeMap {
    private:
        uint32_t num_pokemon;
        double total_dist = 0;
        vector<pair<double, double> > part_B_vec;
        vector<size_t> part_B_output;

    public:
    void B_read_input() {
        cin >> num_pokemon;
        part_B_vec.reserve(num_pokemon);
        int x_cord;
        int y_cord;
        while (cin >> x_cord >> y_cord) {
            part_B_vec.push_back({x_cord, y_cord});
        }
    }

    double B_dist_helper(pair<double, double> &p1, pair<double, double> &p2) {
        double x_dist_sqrd = (p2.first - p1.first) * (p2.first - p1.first);
        double y_dist_sqrd = (p2.second - p1.second) * (p2.second - p1.second);
        return sqrt(x_dist_sqrd + y_dist_sqrd);
    }

    void arbitrary_insertion() {
        part_B_output.reserve(num_pokemon);
        for (size_t i = 0; i < num_pokemon; ++i) {
            part_B_output[i] = i;
        }
        for (size_t i = 0; i < num_pokemon - 1; ++i) {
            total_dist += B_dist_helper(part_B_vec[i], part_B_vec[i + 1]);
        }
        total_dist += B_dist_helper(part_B_vec[0], part_B_vec[num_pokemon - 1]);
    }

    void part_B_print() {
        cout << total_dist << '\n';
        for (size_t i = 0; i < num_pokemon; ++i) {
            cout << part_B_output[i] << " ";
        }
    }

};

class C_PokeMap {
    private:

        struct PrimNode {
            bool visited = false;
            double dist;
            size_t pred;

            PrimNode(bool visited_in, double dist_in, size_t pred_in) : 
                    visited(visited_in), dist(dist_in), pred(pred_in) {}
        };
        
        vector<pair<double, double> > coordinates;
        vector<size_t> cur_path;
        vector<size_t> best_path;
        double cur_path_len = 0;
        //upper-bound
        double best_path_len = 0;
        size_t num_pokemon;

    public:

    void C_read_input() {
        cin >> num_pokemon;
        cur_path.reserve(num_pokemon);
        best_path.reserve(num_pokemon);
        int x_cord;
        int y_cord;
        while (cin >> x_cord >> y_cord) {
            coordinates.push_back({x_cord, y_cord});
        }
    }

    double C_dist_helper(pair<double, double> &p1, pair<double, double> &p2) {
        double x_dist_sqrd = (p2.first - p1.first) * (p2.first - p1.first);
        double y_dist_sqrd = (p2.second - p1.second) * (p2.second - p1.second);
        return sqrt(x_dist_sqrd + y_dist_sqrd);
    }

    void nearest_insertion() {
        cur_path.reserve(num_pokemon);
        vector<size_t> not_in_yet;
        cur_path.push_back(0);
        not_in_yet.reserve(num_pokemon - 1);

        for (size_t i = 1; i < num_pokemon; ++i) {
            not_in_yet.push_back(i);
        }

        size_t next = num_pokemon;
        double cur_min = numeric_limits<double>::infinity();
        //ADD SECOND ELEMENT
        for (size_t i = 1; i < num_pokemon; ++i) {
            double new_dist = C_dist_helper(coordinates[0], coordinates[i]);
            if (new_dist < cur_min) {
                cur_min = new_dist;
                next = i;
            }
        }
        //REMOVE THE INDEX FROM NOT_IN_YET
        cur_path.push_back(next);
        best_path_len += cur_min;
        for (size_t j = 0; j < not_in_yet.size(); ++j) {
            if (not_in_yet[j] == next) {
                not_in_yet[j] = not_in_yet.back();
                not_in_yet.pop_back();
                break;
            }
        }
        size_t prev = next;
        //ADD THIRD ELEMENT
        double cur_min2 = numeric_limits<double>::infinity();
        for (size_t i = 0; i < not_in_yet.size(); ++i) {
            if (not_in_yet[i] != next) {
                double new_dist = C_dist_helper(coordinates[prev], coordinates[not_in_yet[i]]);
                if (new_dist < cur_min2) {
                    cur_min2 = new_dist;
                    next = not_in_yet[i];
                }
            }
        }
        //CREATE CYCLE
        cur_path.push_back(next);
        best_path_len += cur_min2;
        best_path_len += C_dist_helper(coordinates[0], coordinates[next]);
        //REMOVE THE INDEX FROM NOT_IN_YET
        for (size_t j = 0; j < not_in_yet.size(); ++j) {
            if (not_in_yet[j] == next) {
                not_in_yet[j] = not_in_yet.back();
                not_in_yet.pop_back();
                break;
            }
        }

        if (num_pokemon == 3) {
            return;
        }

        //START LOOPING THROUGH THE REST
        size_t size = not_in_yet.size();
        for (size_t loop = 0; loop < size; ++loop) {
            size_t prev = next;
            //STEP 2: FIND NEW NODE TO ADD
            double closest = numeric_limits<double>::infinity();
            for (size_t k = 0; k < not_in_yet.size(); ++k) {
                double cur_min3 = C_dist_helper(coordinates[prev], coordinates[not_in_yet[k]]);
                if (cur_min3 < closest) {
                    next = not_in_yet[k];
                    closest = cur_min3;
                }
            }
            //ADD TO TOTAL
            best_path_len += closest;
            //REMOVE NEW NODE FROM NOT IN YET
            for (size_t j = 0; j < not_in_yet.size(); ++j) {
                if (not_in_yet[j] == next) {
                    not_in_yet[j] = not_in_yet.back();
                    not_in_yet.pop_back();
                    break;
                }
            }
            //STEP 3: DETERMINE WHICH OTHER NODE IN THE CYCLE IS CLOSEST
            double already_in = numeric_limits<double>::infinity();
            size_t to_delete = num_pokemon;
            for (size_t i = 0; i < cur_path.size(); ++i) {
                double new_edge = C_dist_helper(coordinates[next], coordinates[cur_path[i]]);
                if (new_edge < already_in) {
                    to_delete = cur_path[i]; 
                    already_in = new_edge;
                }
            }
            cur_path.push_back(next);
            best_path_len += already_in;
            //GET RID OF THE EXTRA EDGE
            best_path_len -= C_dist_helper(coordinates[to_delete], coordinates[prev]);
        }
        best_path = cur_path;
    }

    double C_construct_prim(size_t permLength) {
        //Initializing prim
        vector<PrimNode> C_prim_vec;
        size_t size = cur_path.capacity() - permLength;
        C_prim_vec.reserve(size);
        PrimNode pn(false, numeric_limits<double>::infinity(), num_pokemon);
        for (size_t i = 0; i < size; ++i) {
            C_prim_vec.push_back(pn);
        }
        PrimNode prim_root(false, 0, num_pokemon);
        C_prim_vec[0] = prim_root;

        double prim_dist = 0;
        size_t index = 0;
        for (size_t i = 0; i < size; ++i) {
            //STEP 1
            double min_dist = numeric_limits<double>::infinity();
            for (size_t j = 0; j < size; ++j) {
                if (C_prim_vec[j].visited == false) {
                    if (C_prim_vec[j].dist < min_dist) {
                        min_dist = C_prim_vec[j].dist;
                        index = j;
                    }
                } 
            }
            //STEP 2
            C_prim_vec[index].visited = true;
            prim_dist += C_prim_vec[index].dist;
            //STEP 3
            for (size_t j = 0; j < size; ++j) {
                if (C_prim_vec[j].visited == false) {
                    double new_dist = C_dist_helper(coordinates[cur_path[permLength + index]], coordinates[cur_path[permLength + j]]);
                    if (new_dist < C_prim_vec[j].dist) {
                        if (j != index) {
                            C_prim_vec[j].dist = new_dist;
                            C_prim_vec[j].pred = index;
                        }
                    }
                }
            }
        }
        return prim_dist;
    }
    
    void genPerms(size_t permLength) {
        //PERMLENGTH = FIXED REGION
        if (permLength == cur_path.size()) {
            //UPDATE
            double first_to_last = C_dist_helper(coordinates[cur_path.front()], coordinates[cur_path.back()]);
            if (cur_path_len + first_to_last < best_path_len) {
                best_path_len = cur_path_len + first_to_last;
                best_path = cur_path;
            }
            return;
        } // if

        if (!promising(permLength))
            return;

        for (size_t i = permLength; i < cur_path.size(); ++i) {
            swap(cur_path[permLength], cur_path[i]);
            cur_path_len += C_dist_helper(coordinates[cur_path[permLength -1]], coordinates[cur_path[permLength]]);
            genPerms(permLength + 1);
            cur_path_len -= C_dist_helper(coordinates[cur_path[permLength -1]], coordinates[cur_path[permLength]]);
            swap(cur_path[permLength], cur_path[i]);
        } // for
    } // genPerms()

        //bounding function
    bool promising(size_t permLength) {
        //TODO: if unvisited.size()! < 10 * (unvisited.size() * unvisited.size() + (2 * unvisited.size()))
        if (cur_path.size() - permLength < 6) {
            return true;
        }
        else {
            double unvisited_MST = C_construct_prim(permLength);
            double arm_1_len = numeric_limits<double>::infinity();
            double arm_2_len = numeric_limits<double>::infinity();
            size_t size = cur_path.size() - permLength;
            for (size_t i = 0; i < size; ++i) {
                double new_dist1 = C_dist_helper(coordinates[cur_path.front()], coordinates[cur_path[permLength + i]]);
                double new_dist2 = C_dist_helper(coordinates[cur_path[permLength - 1]], coordinates[cur_path[permLength + i]]);
                if (new_dist1 < arm_1_len) {
                    arm_1_len = new_dist1;
                }
                if (new_dist2 < arm_2_len) {
                    arm_2_len = new_dist2;
                }
            }
            if ((cur_path_len + unvisited_MST + arm_1_len + arm_2_len) < best_path_len) {
                return true;
            }
        }
        return false;
    }

    void part_C_print() {
        cout << best_path_len << '\n';
        for (size_t i = 0; i < num_pokemon; ++i) {
            cout << best_path[i] << " ";
        }
    }
};

void get_options(int argc, char * argv[]) {
    int gotopt = 0;
    int option_index = 0;

    option long_opts [] = {
        {"help", no_argument,       nullptr, 'h'},
        {"mode", required_argument, nullptr, 'm'}
    };

    while ((gotopt = getopt_long(argc, argv, "hm:", long_opts, &option_index)) != -1) {
        switch (gotopt) {
            case 'h':
                cout << "helpful message\n";
                exit(0);
                break;
            case 'm':
                a_b_c = optarg;
                if (a_b_c == "MST") {
                    mst = true;
                    continue;
                }
                else if (a_b_c == "FASTTSP") {
                    fasttsp = true;
                }
                else if (a_b_c =="OPTTSP") {
                    opttst = true;
                }
                else {
                    //TODO: ERROR
                    exit(1);
                }
                break;
            default:
                cout << "Error: invalid option" << endl;
                exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    cout << setprecision(2);
    cout << fixed;
    get_options(argc, argv);
    if (mst) {
        A_PokeMap A_pm;
        A_pm.A_read_input();
        A_pm.A_construct_prim();
        A_pm.part_A_print();
    }
    else if (fasttsp) {
        B_PokeMap B_pm;
        B_pm.B_read_input();
        B_pm.arbitrary_insertion();
        B_pm.part_B_print();
    }
    else {
        C_PokeMap C_pm;
        C_pm.C_read_input();
        C_pm.nearest_insertion();
        C_pm.genPerms(1);
        C_pm.part_C_print();
    }
    return 0;
}