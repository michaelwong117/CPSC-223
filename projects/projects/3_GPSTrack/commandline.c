track_merge (dest=0x407480, src=0x4074c0) at track.c:400
400	  while (srccurr != &src->tail && destcurr != &dest->tail)
(gdb) p srccurr
$60 = (track_node *) 0x407498
(gdb) p &dest->tail
$61 = (track_node *) 0x407498
(gdb) p destcurr
$62 = (track_node *) 0x4074d8
(gdb) p &dest->tail
$63 = (track_node *) 0x407498

// accidently switched src and dest without switching src->tail and dest->tail

./GPS -closest closest_bottom.in closest_top.in 
./GPS -combine input1.in input2.in
gdb --args ./TrackUnit 100 1 1
b track.c:462
watch srccurr
watch srccurr->data
watch destcurr
watch destcurr->data

valgrind --leak-check=full --show-leak-kinds=all ./TrackUnit 1


/c/cs223/bin/submit 3 GPS.c track.c log makefile
/c/cs223/bin/testit 3 GPS

if (trackpoint_compare(dest->head.next->data, src->head.next->data) == -1 && trackpoint_compare(dest->tail.prev->data, src->tail.prev->data) == -1)
 {
   shared1(dest, &curr1, &curr2)
 }


track_add_point is false so it's not adding trackpoint, but essentially is according to track_unit.c's test_timing merge


Starting program: /home/classes/cs223/class/wong.michael.mjw223/projects/projects/3_GPSTrack/TrackUnit 100 1 1

Breakpoint 1, test_timing_merge (k=1, on=true) at track_unit.c:486
warning: Source file is more recent than executable.
486	  for (size_t i = 0; i < k * k; i++)
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.33-20.fc34.x86_64
(gdb) s

Breakpoint 2, test_timing_merge (k=1, on=true) at track_unit.c:488
488	      location *loc1 = location_create(41.6, -76.7);
(gdb) n

Breakpoint 3, test_timing_merge (k=1, on=true) at track_unit.c:489
489	      trackpoint *pt1 = loc1 != NULL ? trackpoint_create(loc1, i) : NULL;

(gdb) 
test_timing_merge (k=1, on=true) at track_unit.c:490
490	      if (pt1 != NULL)
(gdb) display pt1
1: pt1 = (trackpoint *) 0x409340
(gdb) display loc1
2: loc1 = (location *) 0x409320
(gdb) display track_add_point(dest,pt1)
3: track_add_point(dest,pt1) = true
(gdb) 
(gdb) s

Breakpoint 4, test_timing_merge (k=1, on=true) at track_unit.c:492
492	    	  track_add_point(dest, pt1);
1: pt1 = (trackpoint *) 0x409340
2: loc1 = (location *) 0x409320
3: track_add_point(dest,pt1) = false
(gdb) display i
4: i = 0
(gdb) display dest
5: dest = (track *) 0x4092a0

test_timing_merge:track_unit.c
  if (pt1 != NULL)
	{
	  track_add_point(dest, pt1); doesn't account for if this is false
	}
  else
	{
	  if (pt1 != NULL)
	    {
	      trackpoint_destroy(pt1);
	    }
	  else
	    {
	      location_destroy(loc1);
	    }
	}

Debug:

Starting program: /home/classes/cs223/class/wong.michael.mjw223/projects/projects/3_GPSTrack/TrackUnit 100 1 1

Breakpoint 1, test_timing_merge (k=1, on=true) at track_unit.c:524
warning: Source file is more recent than executable.
524	
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.33-20.fc34.x86_64

head chain
0x4092e->0x4093c0->0x4092f8->0x0
tail chain
0x0<-0x4092e0<-0x4093c0<-0x4092f8

(gdb) p &src->head
$8 = (track_node *) 0x4092e
(gdb) p src->head.next
$1 = (struct _track_node *) 0x4093c0
(gdb) p src->head.next->next
$2 = (struct _track_node *) 0x4092f8
(gdb) p src->head.next->next->next
$3 = (struct _track_node *) 0x0
(gdb) p src->head.next->next->nextQuit
(gdb) p &src->tail
$4 = (track_node *) 0x4092f8
(gdb) p src->tail.prev
$5 = (struct _track_node *) 0x4093c0
(gdb) src->tail.prev->prev
Undefined command: "src->tail.prev->prev".  Try "help".
(gdb) p src->tail.prev->prev
$6 = (struct _track_node *) 0x4092e0
(gdb) p src->tail.prev->prev->prev
$7 = (struct _track_node *) 0x0