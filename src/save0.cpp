#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

string D = "NEWSX";
int SIZE = 1 << 6;
int dr[] = {-1, 0, 0, 1, 0};
int dc[] = {0, 1, -1, 0, 0};
int dd[] = {-SIZE, 1, -1, SIZE, 0};
int start[] = {0, 0, 0, 0};
int s, t, te, k, g, centor, cd;
int group[1 << 7];
int pos[1 << 7];
int target[1 << 7];
int to[1 << 7];

int get_pos(int r, int c) { return (r << 6) | c; }

int dist(int a, int b) {
  int ar = a >> 6;
  int ac = a & (SIZE - 1);
  int br = b >> 6;
  int bc = b & (SIZE - 1);
  return abs(ar - br) + abs(ac - bc);
}

int get_random_pos() {
  int r = get_random() % s;
  int c = get_random() % s;
  return get_pos(r, c);
}

int get_random_pos2() {
  while (true) {
    int p = get_random_pos();
    if (dist(centor, p) <= cd) return p;
  }
}

int too(int a, int b) {
  assert(a != b);
  static int d[4];
  int di = 0;
  int x = dist(a, b);
  for (int i = 0; i < 4; ++i) {
    if (x > dist(a + dd[i], b)) d[di++] = i;
  }
  return d[get_random() % di];
}

class PrincessesAndMonsters {
 public:
  string initialize(int S, vector<int>& p, vector<int>& m, int K) {
    s = S;
    k = K;
    g = 4;
    t = -1;
    te = S * S * S - S * 2;
    centor = get_pos(s / 2, s / 2);
    cd = 0;
    for (int i = 0; i < p.size(); i += 2) {
      int td = dist(centor, get_pos(p[i], p[i + 1])) + 2;
      if (cd < td) cd = td;
    }
    start[0] = get_pos(0, 0);
    start[1] = get_pos(0, s - 1);
    start[2] = get_pos(s - 1, s - 1);
    start[3] = get_pos(s - 1, 0);
    string s = string(K, '0');
    for (int i = 0; i < g; ++i) {
      pos[i] = target[i] = start[i % 4];
    }
    for (int i = 0; i < k; ++i) {
      group[i] = i % g;
      s[i] = '0' + (group[i] % 4);
    }
    return s;
  }
  string move(vector<int>& status, int P, int M, int timeLeft) {
    int tp = P;
    for (int i : status)
      if (i > 0) tp -= i;
    t++;
    for (int i = 0; i < g; ++i) {
      if (t > 0 && pos[i] == 0) continue;
      if (te <= t || (tp == 0 && M == 0))
        target[i] = 0;
      else {
        while (pos[i] == target[i]) target[i] = get_random_pos2();
      }
      to[i] = too(pos[i], target[i]);
      pos[i] += dd[to[i]];
    }
    string s = string(k, D[0]);
    for (int i = 0; i < k; ++i) {
      s[i] = D[to[group[i]]];
    }
    return s;
  }
};

// -------8<------- end of solution submitted to the website -------8<-------
template <class T>
void getVector(vector<T>& v) {
  for (int i = 0; i < v.size(); ++i) cin >> v[i];
}

int main() {
  PrincessesAndMonsters pam;

  int S, P, M, K;
  cin >> S >> P;
  vector<int> princesses(P);
  getVector(princesses);
  cin >> M;
  vector<int> monsters(M);
  getVector(monsters);
  cin >> K;

  string retInit = pam.initialize(S, princesses, monsters, K);
  cout << retInit << endl;
  cout.flush();

  for (int st = 0;; ++st) {
    int nK;
    cin >> nK;
    vector<int> status(K);
    getVector(status);
    int nP, nM;
    cin >> nP >> nM;
    int timeLeft;
    cin >> timeLeft;

    string ret = pam.move(status, nP, nM, timeLeft);
    cout << ret << endl;
    cout.flush();
  }
}