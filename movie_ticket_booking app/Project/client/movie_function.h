#ifndef MOVIEFUNC_H
#define MOVIEFUNC_H

int search_movie_by_title(int socketfd);
int browse_movie(int socketfd, int category);
int view_movie_detail(int socketfd);
#endif