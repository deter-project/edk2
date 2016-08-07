let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/edk2/DeterPkg
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +1 ~/edk2/NetworkPkg/HttpBootDxe/HttpBootDxe.inf
badd +1 ~/edk2/NetworkPkg/HttpBootDxe/HttpBootClient.h
badd +1 ~/edk2/NetworkPkg/HttpBootDxe/HttpBootClient.c
argglobal
silent! argdel *
argadd ~/edk2/DeterPkg
edit ~/edk2/NetworkPkg/HttpBootDxe/HttpBootClient.h
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
exe '1resize ' . ((&lines * 45 + 47) / 94)
exe 'vert 1resize ' . ((&columns * 103 + 103) / 206)
exe '2resize ' . ((&lines * 46 + 47) / 94)
exe 'vert 2resize ' . ((&columns * 103 + 103) / 206)
exe 'vert 3resize ' . ((&columns * 102 + 103) / 206)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 129 - ((38 * winheight(0) + 22) / 45)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
129
normal! 0
lcd ~/edk2/DeterPkg
wincmd w
argglobal
edit ~/edk2/NetworkPkg/HttpBootDxe/HttpBootClient.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 662 - ((22 * winheight(0) + 23) / 46)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
662
normal! 0
lcd ~/edk2/DeterPkg
wincmd w
argglobal
enew
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
lcd ~/edk2/DeterPkg
wincmd w
2wincmd w
exe '1resize ' . ((&lines * 45 + 47) / 94)
exe 'vert 1resize ' . ((&columns * 103 + 103) / 206)
exe '2resize ' . ((&lines * 46 + 47) / 94)
exe 'vert 2resize ' . ((&columns * 103 + 103) / 206)
exe 'vert 3resize ' . ((&columns * 102 + 103) / 206)
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
