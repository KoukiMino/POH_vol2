#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string.h>
using namespace std;

const std::size_t INPUT_BUF_SIZE = 90002 * 8 + 90601;

int W;

char* g_pInputBuf;
char* g_pInputBufPtr;
//char* g_pOutputBuf;
char* g_pOutputBufPtr;

int _read_int(){
  int r=0;
  while('\n' != *g_pInputBufPtr && ' ' != *g_pInputBufPtr && '\0' != *g_pInputBufPtr ){
      r = r * 10 + *g_pInputBufPtr - '0';
      ++g_pInputBufPtr;
    }
    ++g_pInputBufPtr;
  return r;
}

void _buf_or(char* buf, char* wbuf, int *list){
  int i=0;
  int zero_cnt=0;
  while (i<W){
    if (*wbuf == *(buf + i)) {
      if (*wbuf == '0'){
        ++zero_cnt;
      }
      else {
        if (zero_cnt>0){
          //list[zero_cnt] += 1;
          *(list + zero_cnt) += 1;
          zero_cnt=0;
        }
      }
    }
    else {
      *wbuf = '1';
      if (zero_cnt>0){
        //list[zero_cnt] += 1;
        *(list + zero_cnt) += 1;
        zero_cnt=0;
      }
    }
    ++wbuf;
    //++buf;
    ++i;
  }
  if (zero_cnt>0){
    //list[zero_cnt] += 1;
    *(list + zero_cnt) += 1;
  }
  //*(wbuf + i) = '\0';
}

int _get_pos_zero(char* cbuf, int pos){
  int ret = -1;
  while (pos<W){
    if (*(cbuf + pos) == '0'){
      ret = pos;
      break;
    }
    ++pos;
  }
  return ret;
}

int _get_pos_one(char* cbuf, int pos){
  int ret = -1;
  while (pos<W){
    if (*(cbuf + pos) == '1'){
      ret = pos;
      break;
    }
    ++pos;
  }
  return ret;
}

int _out_int (int cnt){
  int first_val = cnt;
  char* first_ptr = g_pOutputBufPtr;
  div_t divresult;
  if (10000 <= cnt){
    divresult = div(cnt,10000);
    *(g_pOutputBufPtr++) = '0' + divresult.quot;
    cnt = divresult.rem;
    //++g_pOutputBufPtr;
  }
  if (1000 <= cnt){
    divresult = div(cnt,1000);
    *(g_pOutputBufPtr++) = '0' + divresult.quot;
    cnt = divresult.rem;
    //++g_pOutputBufPtr;
  }
  else {
    if (10000 <= first_val){
      *(g_pOutputBufPtr++) = '0';
      //++g_pOutputBufPtr;
    }
  }
  if (100 <= cnt){
    divresult = div(cnt,100);
    *(g_pOutputBufPtr++) = '0' + divresult.quot;
    cnt = divresult.rem;
    //++g_pOutputBufPtr;
  }
  else {
    if (1000 <= first_val){
      *(g_pOutputBufPtr++) = '0';
      //++g_pOutputBufPtr;
    }
  }
  if (10 <= cnt){
    divresult = div(cnt,10);
    *(g_pOutputBufPtr++) = '0' + divresult.quot;
    cnt = divresult.rem;
    //++g_pOutputBufPtr;
  }
  else {
    if (100 <= first_val){
      *(g_pOutputBufPtr++) = '0';
      //++g_pOutputBufPtr;
    }
  }
  *(g_pOutputBufPtr++) = '0' + cnt;
  //++g_pOutputBufPtr;
  *(g_pOutputBufPtr++) = 10;
  //++g_pOutputBufPtr;
  return g_pOutputBufPtr - first_ptr;
}

int main() {
  g_pInputBuf = static_cast<char *>(std::malloc(INPUT_BUF_SIZE));
  g_pInputBufPtr  = g_pInputBuf;
  std::fread(g_pInputBuf, 1, INPUT_BUF_SIZE, stdin);
 
  int H = _read_int();
  W = _read_int();
  int HP = H + 1;
  int WP = W + 1;
  // cout << "H = " << H << " , W = " << W << "\n";

  int i,j,k,SH,TW,pos,first,second,key,cnt;

  char* check_block[H];
  int check_list[HP][WP];
  int key_list[HP][WP];
  int *keyp;
  int *checkp;

  for (j=0;j<H;++j){
    for (i=WP;i>0;--i){
      check_list[j+1][i]=0;
      key_list[j+1][i]=0;
    }
    check_block[j] = static_cast<char *>(std::malloc(WP));
    memcpy(check_block[j], g_pInputBufPtr, W);
    g_pInputBufPtr += WP;
    pos = 0;
    while (pos<W){
      first = _get_pos_zero(check_block[j],pos);
      // cout << "first ->" << first << "\n";
      if (first>=0){
          second = _get_pos_one(check_block[j],first);
          // cout << "second ->" << second << "\n";
          if (second >= 0) {
            key = second - first;
            pos = second;
          }
          else {
            key = W - first;
            pos = W;
          }
          // cout << "key ->" << key << "\n";
          ++check_list[1][key];
      }
      else {
        break;
      }
    }
  }

  keyp = &key_list[1][0];
  k = W;
  checkp = &check_list[1][k];
  while (k>0){
    if (*(checkp--)>0){
      *(keyp++) = k;
    }
    --k;
  }

  for (SH=2;SH<=H;++SH){
    k=H-SH;
    for (j=0;j<=k;++j){
      _buf_or(check_block[j+1],check_block[j],&check_list[SH][0]);
    }
    keyp = &key_list[SH][0];
    k = W;
    checkp = &check_list[SH][k];
    while (k>0){
      if (*(checkp--)>0){
        *(keyp++) = k;
      }
      --k;
    }
  }

  // for (j=0;j<H;++j){
  //    for (i=0;i<W;++i){
  //       cout << "SH ->" << (j+1) << " TW ->" << (i+1) << " count ->" << check_list[j][i] << "\n";
  //    }
  // }

  int N = _read_int();
  int oi = 0;
  g_pOutputBufPtr  = g_pInputBuf;
  for (i=N-1;i>=0;--i){
    SH = _read_int();
    TW = _read_int();
    // cout << "SH ->" << SH << " TW ->" << TW << "\n";
    cnt = 0;
    if (SH<=H && TW<=W){
      keyp = &key_list[SH][0];
      checkp = &check_list[SH][0];
      while (1){
        key = *(keyp++);
        if (key<TW) break;
        cnt += (key - TW + 1) * *(checkp + key);
      }
    }
    //cout << cnt << "\n";
    oi += _out_int(cnt);
  }
  //*g_pOutputBufPtr = '\0';
  std::fwrite(g_pInputBuf, 1, oi, stdout);
  return 0;
}
