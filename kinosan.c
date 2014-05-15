#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/time.h>

#define INPUT_BUF_SIZE	90002 * 8 + 90601

int	_read_int();
void _buf_or(char* buf1, char* buf2, char* wbuf, int *list);
int _get_pos_zero(char* cbuf, int pos);
int _get_pos_one(char* cbuf, int pos);
int _out_int (int cnt);

int W;

char*	g_pInputBuf;
char*	g_pInputBufPtr;
char*	g_pOutputBuf;
char*	g_pOutputBufPtr;

int main()
{
    //struct timeval s, e;
    //gettimeofday(&s, NULL);

	g_pInputBuf	= ( char* )malloc( INPUT_BUF_SIZE );
	g_pInputBufPtr	= g_pInputBuf;
	fread( g_pInputBuf, 1, INPUT_BUF_SIZE, stdin );

	int H = _read_int();
	W = _read_int();
	int HM = H - 1;
	int WP = W + 1;
	// printf("H->%d W->%d\n", H, W);

	int i,j,k,SH,TW,pos,first,second,key,cnt;
	char* check_block[H];
	int check_list[H][WP];
	char* bin_home_list;
	bin_home_list	= ( char* )malloc(WP);
	bin_home_list[W] = '\0';
	int key_list[H][WP];
	int key_count[H];
	int *keyp;
	int *checkp;

	for (j=0;j<H;++j){
		check_block[j] = ( char* )malloc(WP);
	 	for (i=WP;i>0;--i){
			check_list[j][i]=0;
			key_list[j][i]=0;
	 	}
	 	key_count[j]=0;

		// printf("-----j->%d\n", j);
		memcpy(bin_home_list, g_pInputBufPtr, W);
		// printf("bin_home_list>%s \n", bin_home_list);
		g_pInputBufPtr += WP;

		for (SH=j;SH>=1;--SH){
			memcpy(check_block[SH], bin_home_list, WP);
			_buf_or(bin_home_list,check_block[SH-1],check_block[SH],&check_list[SH][0]);

			if (j==HM){
				keyp = &key_list[SH][0];
				checkp = &check_list[SH][0];
				for (i=W;i>0;--i){
					if (*(checkp + i)>0){
						*(keyp + key_count[SH]++) = i;
						//key_list[SH][key_count[SH]++]=i;
					}
				}
			}
		}

		memcpy(check_block[0], bin_home_list, WP);

		pos = 0;
		while (pos<W){
			first = _get_pos_zero(bin_home_list,pos);
			// printf("first>%d \n", first);
			if (first >= 0){
				second = _get_pos_one(bin_home_list,first);
				// printf("second>%d \n", second);
				if (second >= 0) {
					key = second - first;
					pos = second;
				}
				else {
					key = W - first;
					pos = W;
				}
				// printf("key>%d \n", key);
				++check_list[0][key];
			}
			else {
				break;
			}
		}
		if (j==HM){
			keyp = &key_list[0][0];
			checkp = &check_list[0][0];
			for (i=W;i>0;--i){
				if (*(checkp + i)>0){
					*(keyp + key_count[0]++)=i;
				}
			}
		}
	}

    //gettimeofday(&e, NULL);
    //printf("prepared time = %lf\n", (e.tv_sec - s.tv_sec) + (e.tv_usec - s.tv_usec)*1.0E-6);

	// for (j=0;j<H;j++){
	//   	for (i=1;i<WP;i++){
	// 	printf("SH = {%d} TW = {%d}  count = {%d} \n", (j+1), i, check_list[j][i] );
	//   	}
	// }
	
	int N = _read_int();
	int oi = 0;
	int SHM;
	g_pOutputBuf = ( char* )malloc( N * 6 );
	g_pOutputBufPtr  = g_pOutputBuf;
	for (i=N-1;i>=0;--i){
		SH = _read_int();
		TW = _read_int();
		// printf("SH = {%d} TW = {%d} \n", SH, TW );
		cnt = 0;
		if (SH<=H && TW<=W){
			k=0;
			SHM = SH - 1;
			keyp = &key_list[SHM][0];
			checkp = &check_list[SHM][0];
			while (1){
				key = *(keyp + k++);
				//key = key_list[SHM][k++];
				if (key<TW)	break;
				cnt += (key - TW + 1) * *(checkp + key);
				//cnt += (key - TW + 1) * check_list[SHM][key];
			}
		}
	    oi += _out_int(cnt);
	}
	//*g_pOutputBufPtr = '\0';
	fwrite(g_pOutputBuf, 1, oi, stdout);

    //gettimeofday(&e, NULL);
    //printf("total time = %lf\n", (e.tv_sec - s.tv_sec) + (e.tv_usec - s.tv_usec)*1.0E-6);

    return 0;
}

int _read_int(){
	int r=0;
	while('\n' != *g_pInputBufPtr && ' ' != *g_pInputBufPtr && '\0' != *g_pInputBufPtr ){
		r = r * 10 + *g_pInputBufPtr - '0';
		++g_pInputBufPtr;
	}
	++g_pInputBufPtr;
	return r;
}

void _buf_or(char* buf1, char* buf2, char* wbuf, int *list){
	int i=0;
	int zero_cnt=0;
	int k;
	while (i<W){
		if (*(buf1 + i) == *(buf2 + i)) {
			//*(wbuf + i) = *(buf1 + i);
			if (*(wbuf + i)=='0'){
				++zero_cnt;
			}
			else {
				if (zero_cnt>0){
					list[zero_cnt] += 1;
					zero_cnt=0;
				}
			}
		}
		else {
			*(wbuf + i) = '1';
			if (zero_cnt>0){
				list[zero_cnt] += 1;
				zero_cnt=0;
			}
		}
		++i;
	}
	if (zero_cnt>0){
		list[zero_cnt] += 1;
	}
	//*(wbuf + i) = '\0';
}

int _get_pos_zero(char* cbuf, int pos){
	int ret = -1;
	while (pos<W){
		// printf("poszero->%d {%c}", pos, *cbuf );
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
		// printf("posone->%d {%c}", pos, *cbuf );
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
    *g_pOutputBufPtr = '0' + divresult.quot;
    cnt = divresult.rem;
    ++g_pOutputBufPtr;
  }
  if (1000 <= cnt){
    divresult = div(cnt,1000);
    *g_pOutputBufPtr = '0' + divresult.quot;
    cnt = divresult.rem;
    ++g_pOutputBufPtr;
  }
  else {
    if (10000 <= first_val){
      *g_pOutputBufPtr = '0';
      ++g_pOutputBufPtr;
    }
  }
  if (100 <= cnt){
    divresult = div(cnt,100);
    *g_pOutputBufPtr = '0' + divresult.quot;
    cnt = divresult.rem;
    ++g_pOutputBufPtr;
  }
  else {
    if (1000 <= first_val){
      *g_pOutputBufPtr = '0';
      ++g_pOutputBufPtr;
    }
  }
  if (10 <= cnt){
    divresult = div(cnt,10);
    *g_pOutputBufPtr = '0' + divresult.quot;
    cnt = divresult.rem;
    ++g_pOutputBufPtr;
  }
  else {
    if (100 <= first_val){
      *g_pOutputBufPtr = '0';
      ++g_pOutputBufPtr;
    }
  }
  *g_pOutputBufPtr = '0' + cnt;
  ++g_pOutputBufPtr;
  *g_pOutputBufPtr = 10;
  ++g_pOutputBufPtr;
  return g_pOutputBufPtr - first_ptr;
}
