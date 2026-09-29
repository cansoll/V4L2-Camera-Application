#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>

int main(void)
{
	int fd;
	struct v4l2_format fmt ={0};
	struct v4l2_requestbuffers req = {0};
	
	
	void *start[4];
	unsigned int  lenght[4];

	unsigned int i;
	unsigned int mapped = 0;
	int result = 0;

	enum v4l2_buf_type type1 = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	struct v4l2_buffer frame = {0};


	FILE *photo;
	size_t written;
	int close_recult;
	
	
	fd = open("/dev/video1",O_RDWR);
	
	if(fd == -1)
	{
		perror("open camera");
		return 1;
	}
	
	printf("camera opened,fd=%d\n",fd);

	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = 640;
	fmt.fmt.pix.height = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.field = V4L2_FIELD_ANY;

	if(ioctl(fd,VIDIOC_S_FMT,&fmt) == -1)
	{
		perror("set format");
		close(fd);
		return 2;
	}
	printf("Image size: %u x %u\n",fmt.fmt.pix.width, fmt.fmt.pix.height);




	req.count = 4;
	req.memory = V4L2_MEMORY_MMAP;	
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl(fd,VIDIOC_REQBUFS,&req) == -1)
	{
		perror("need req");
		close(fd);
		return 3;
	}

	if(req.count == 0)
	{
		printf("no reqbuffer");
		close(fd);
		return 4;
	}
	printf("buffer count = %d\n",req.count);





	for(i = 0; i < req.count; i++)
	{
		struct v4l2_buffer buf = {0};
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;		
		
		if(ioctl(fd,VIDIOC_QUERYBUF,&buf) == -1)
		{
			perror("no buffer");
			close(fd);
			return 5;
			break;
		}
		lenght[i] = buf.length; 
		printf("buffer index = %u\n",buf.index);
		printf("buffer lenght = %ub\n",lenght[i]);
		printf("Buffer offset: %u\n", buf.m.offset);


		start[i] = mmap(NULL, buf.length,
	             PROT_READ | PROT_WRITE,
	             MAP_SHARED, fd, buf.m.offset);
		if(start[i] == MAP_FAILED)
		{
			perror("mmap");
			close (fd);
			return 6;
			break;
		}

		if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)
		{
			perror("queue buffer");
			return 7;
			break;
		}

		printf("buffer %u queued\n",i);
		
		mapped++;
		printf("buffer i = %u,buffer address = %p,lenght = %u\n",i,start[i],lenght[i]);
	}


	if(result == 0)
	{
		if(ioctl(fd,VIDIOC_STREAMON,&type1) == -1)
		{
			perror("strat capture");
			return 8;
		
		}
		printf("capture strated\n");




		frame.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE;
		frame.memory = V4L2_MEMORY_MMAP;

		if(ioctl(fd,VIDIOC_DQBUF,&frame) == -1)
		{
			perror("get frame");
			return 10;
		}
		printf("frame index = %u\n",frame.index);
		printf("frame bytes = %u\n",frame.bytesused);



		photo = fopen("/root/picture/learn1.jpg","wb");
		if (photo == NULL)
		{
			perror("open photo");
			return 11;
		}
		else
		{
			written = fwrite(start[frame.index], 1, frame.bytesused, photo);
			close_recult = fclose(photo);
			
			if(written != frame.bytesused ||close_recult != 0)
			{
				perror("write");
				return 12;
			}
			else
			{
				printf("file is saved /root/picture/learn1.jpg\n");
			}
		}



		
		if(ioctl(fd,VIDIOC_STREAMOFF,&type1) == -1)
		{
			perror("stop capture");
			return 9;
		}
		printf("capture stopped\n");
	}


	for(i = 0; i < mapped; i++)
	{
		munmap(start[i],lenght[i]);
	}

	
	close(fd);
	printf("camera closed\n");
	
	return result;

}
























