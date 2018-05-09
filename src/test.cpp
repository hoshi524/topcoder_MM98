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

class PrincessesAndMonsters {
 public:
  string initialize(int S, vector<int>& p, vector<int>& m, int K) {
    s = S;
    k = K;
    phase = 0;
    minr = minc = s / 2;
    maxr = maxc = s / 2;
    constexpr int range = 2;
    for (int i = 0; i < p.size(); i += 2) {
      int r = p[i];
      int c = p[i + 1];
      if (minr > r - range) minr = r - range;
      if (maxr < r + range) maxr = r + range;
      if (minc > c - range) minc = c - range;
      if (maxc < c + range) maxc = c + range;
    }
    {
      memset(map_rev, 0, sizeof(map_rev));
      for (int i = 0; i < s; ++i) map_rev[i] = 1;
    }
    {
      for (int i = 0; i * 2 < s; ++i) {
        for (int j = 0; j < s; ++j) {
          map_goo[get_pos(i, j)] = 3;
          map_goo[get_pos(s - 1 - i, j)] = 0;
        }
      }
      for (int i = 0; i * 2 < s; ++i) {
        map_goo[get_pos(minr, i)] = 1;
        map_goo[get_pos(minr, s - 1 - i)] = 2;
        map_goo[get_pos(maxr, i)] = 1;
        map_goo[get_pos(maxr, s - 1 - i)] = 2;
      }
      tr = s / 2;
      if (tr < minr) tr = minr;
      if (tr > maxr) tr = maxr;
      for (int i = 0; i * 2 < s; ++i) {
        map_goo[get_pos(tr, i)] = 1;
        map_goo[get_pos(tr, s - 1 - i)] = 2;
      }
      tr = get_pos(tr, s / 2);
      map_goo[tr] = 5;
    }
    {
      memset(map_count, 0, sizeof(map_count));
      int t = 0;
      while (true) {
        for (int c = minc; c <= maxc; ++c) {
          map_count[get_pos(minr, c)]++;
          if (++t == k) goto outer;
          map_count[get_pos(maxr, c)]++;
          if (++t == k) goto outer;
        }
      }
    outer:
      memcpy(map_count_mut, map_count, sizeof(map_count));
    }
    start[0] = get_pos(0, 0);
    start[1] = get_pos(0, s - 1);
    start[2] = get_pos(s - 1, s - 1);
    start[3] = get_pos(s - 1, 0);
    string s = string(K, '0');
    {
      int count[] = {0, 0, 0, 0, 0};
      for (int c = minc; c <= maxc; ++c) {
        int p = get_pos(minr, c);
        if (map_goo[p] == 1)
          count[1] += map_count[p];
        else
          count[2] += map_count[p];
        p = get_pos(maxr, c);
        if (map_goo[p] == 1)
          count[4] += map_count[p];
        else
          count[3] += map_count[p];
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
            int r = pos[i] >> 6;
            int to = -1;
            if (r == minr) to = 3;
            if (r == maxr) to = 0;
            if (map_count_mut[pos[i]] > 1 && map_count[pos[i]] == 0) to = 4;
            assert(to > -1);
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