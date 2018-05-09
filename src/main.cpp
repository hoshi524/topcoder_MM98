#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

string D = "NEWSX";
constexpr int SIZE = 1 << 6;
constexpr int ALL = SIZE * SIZE;
int dr[] = {-1, 0, 0, 1, 0};
int dc[] = {0, 1, -1, 0, 0};
int dd[] = {-SIZE, 1, -1, SIZE, 0};
int start[] = {0, 0, 0, 0};
int s, k, t, tr;
int minr, minc;
int maxr, maxc;
int map_rev[ALL];
int map_goo[ALL];
int map_count[ALL];
int map_count_mut[ALL];
int pos[ALL];
int phase;

int get_pos(int r, int c) { return (r << 6) | c; }

int get_dist(int a, int b) {
  int ar = a >> 6;
  int ac = a & 0x3f;
  int br = b >> 6;
  int bc = b & 0x3f;
  return abs(ar - br) + abs(ac - bc);
}

class PrincessesAndMonsters {
 public:
  string initialize(int S, vector<int>& p, vector<int>& m, int K) {
    s = S;
    k = K;
    phase = 0;
    minr = minc = s / 2;
    maxr = maxc = s / 2;
    constexpr int range = 3;
    for (int i = 0; i < p.size(); i += 2) {
      int r = p[i];
      int c = p[i + 1];
      if (minr > r - range) minr = r - range;
      if (maxr < r + range) maxr = r + range;
      if (minc > c - range) minc = c - range;
      if (maxc < c + range) maxc = c + range;
    }

    memset(map_rev, 0, sizeof(map_rev));
    for (int i = 0; i < s; ++i) map_rev[i] = 1;

    tr = s / 2 + 1;
    int D[4][2] = {{3, 1}, {3, 2}, {0, 1}, {0, 2}};
    int P[4][4] = {
        {0, 0, tr, tr},
        {0, tr, tr, s},
        {tr, 0, s, tr},
        {tr, tr, s, s},
    };
    int RC[4][2] = {{minr, minc}, {minr, maxc}, {maxr, minc}, {maxr, maxc}};
    int queue[ALL], qi = 0;
    for (int i = 0; i < 4; ++i) {
      int rl = abs(RC[i][0] - tr);
      int cl = abs(RC[i][1] - tr);
      for (int r = P[i][0]; r < P[i][2]; ++r) {
        for (int c = P[i][1]; c < P[i][3]; ++c) {
          map_goo[get_pos(r, c)] = D[i][rl > cl ? 0 : 1];
        }
      }
      if (rl > cl) {
        for (int c = P[i][1]; c < P[i][3]; ++c) {
          int p = get_pos(RC[i][0], c);
          map_goo[p] = D[i][1];
          if (minr <= RC[i][0] && RC[i][0] <= maxr && minc <= c && c <= maxc) {
            queue[qi++] = p;
          }
        }
      } else {
        for (int r = P[i][0]; r < P[i][2]; ++r) {
          int p = get_pos(r, RC[i][1]);
          map_goo[p] = D[i][0];
          if (minr <= r && r <= maxr && minc <= RC[i][1] && RC[i][1] <= maxc) {
            queue[qi++] = p;
          }
        }
      }
    }
    tr = s / 2;
    for (int i = 0; i * 2 < s; ++i) {
      map_goo[get_pos(tr, i)] = 1;
      map_goo[get_pos(tr, s - 1 - i)] = 2;
      map_goo[get_pos(i, tr)] = 3;
      map_goo[get_pos(s - 1 - i, tr)] = 0;
    }
    tr = get_pos(tr, s / 2);
    map_goo[tr] = 5;

    memset(map_count, 0, sizeof(map_count));
    for (int t = 0; t < k; ++t) {
      map_count[queue[t % qi]]++;
    }
    memcpy(map_count_mut, map_count, sizeof(map_count));

    start[0] = get_pos(0, 0);
    start[1] = get_pos(0, s - 1);
    start[2] = get_pos(s - 1, s - 1);
    start[3] = get_pos(s - 1, 0);
    string s = string(K, '0');
    int count[] = {0, 0, 0, 0, 0};
    int to[4] = {1, 2, 4, 3};
    for (int r = 0; r < S; ++r) {
      for (int c = 0; c < S; ++c) {
        int p = get_pos(r, c);
        if (map_count[p] == 0) continue;
        for (int i = 0; i < 4; ++i) {
          if (P[i][0] <= r && r < P[i][2] && P[i][1] <= c && c < P[i][3]) {
            count[to[i]] += map_count[p];
          }
        }
      }
    }
    for (int i = 1; i < 5; ++i) {
      count[i] += count[i - 1];
    }
    for (int i = 0; i < 4; ++i) {
      for (int j = count[i]; j < count[i + 1]; ++j) {
        s[j] = '0' + i;
        pos[j] = start[i];
      }
    }
    return s;
  }
  string move(vector<int>& status, int P, int M, int timeLeft) {
    string s = string(k, D[0]);
    auto move = [&](int i, int d) {
      s[i] = D[d];
      pos[i] += dd[d];
    };
    if (phase == 0) {
      bool end = true;
      for (int i = 0; i < k; ++i) end &= pos[i] == tr;
      if (end) {
        phase = 1;
      } else {
        for (int i = 0; i < k; ++i) {
          if (map_count[pos[i]] > 0) {
            --map_count[pos[i]];
            auto get_to = [&](int p) {
              int dist = get_dist(p, tr);
              for (int i = 0; i < 4; ++i) {
                if (i == map_goo[p]) continue;
                if (dist > get_dist(p + dd[i], tr)) return i;
              }
              return map_goo[p];
            };
            int to = get_to(pos[i]);
            if (map_count_mut[pos[i]] > 1 && map_count[pos[i]] == 0) to = 4;
            move(i, to);
          } else {
            move(i, map_goo[pos[i]]);
          }
        }
        for (int i = 0; i < k; ++i) {
          if (map_count_mut[pos[i]] > 1 && map_count[pos[i]] == 0) {
            map_count[pos[i]] = 1;
          }
        }
        memcpy(map_count_mut, map_count, sizeof(map_count));
      }
    }
    if (phase == 1) {
      for (int i = 0; i < k; ++i) {
        move(i, map_rev[pos[i]]);
      }
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