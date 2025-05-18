#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> 
#include <errno.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000

int configure_serial(char * file, int bauds) {
  int serial_port = open(file, O_RDWR);
  struct termios tty;
  if (serial_port < 0) {
    fprintf(stderr, "Erreur à l'ouverture du fichier %s\n", file);
    return -1;
  }
  
  // Initialisation de la structure tty
  if(tcgetattr(serial_port, &tty) != 0) {
      fprintf(stderr,"Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return -1;
  }

  //tty.c_cflag &= ~PARENB; // Pas de bit de parité
  tty.c_cflag &= ~CSTOPB; // 1 bit stop
  tty.c_cflag &= ~CSIZE; // 8 bits de données (cleat + set)
  tty.c_cflag |= CS8; 
  tty.c_cflag &= ~CRTSCTS; // Désactiver RTS/CTS
  tty.c_cflag |= CREAD | CLOCAL; // Activation de la lecture et on ignore les inst. de contrôle

  tty.c_lflag |= ICANON; // lecture caractère par caractère et non ligne par ligne
  tty.c_lflag &= ~ECHO; // Pas d'écho
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG; // On n'interpête pas les caractères INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Désactiver le contrôle de flux logiciel
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Pas de traitement des octets de contrôle

  // tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  // tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 100;    // Lecture bloquante, avec un timeout de 10 secondes (unité : 0.1 s)
  tty.c_cc[VMIN] = 0;

  // Vitesse 
  cfsetispeed(&tty, bauds);
  //cfsetospeed(&tty, bauds);

  // Enregistrement des réglages
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      fprintf(stderr,"Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return -1;
  }
  return serial_port; 
}


int main(void) {
  // Buffer pour stocker les données lues
  // Une ligne ne pourra pas faire plus que 256 caractères
  char read_buf [256];
  char buffer[512];
  unsigned char serialbuffer[5];

  int serial_port; 
  int listenfd;
  socklen_t len;
  
  // Effacement du buffer
  memset(&read_buf, '\0', sizeof(read_buf));

  // Ouverture du port série
  serial_port=configure_serial("/dev/ttyACM0", B1800);
  if (serial_port < 0) return 1;

  sleep (3);  // attendre le redemrage du uno

  struct sockaddr_in servaddr, cliaddr;
  bzero(&servaddr, sizeof(servaddr));

  // Create a UDP Socket
  listenfd = socket(AF_INET, SOCK_DGRAM, 0);        
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  servaddr.sin_family = AF_INET; 

  // bind server address to socket descriptor
  bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

  int fd = open("buffer.txt", O_RDONLY );
  read(fd,buffer,512);

  serialbuffer[0] = 255; // start 
  serialbuffer[4] = 254; // end
    
  //receive the datagram
  len = sizeof(cliaddr);
  while(1){

    int n = recvfrom(listenfd, buffer, sizeof(buffer),
            0, (struct sockaddr*)&cliaddr,&len); //receive message from server

    float speed = *((float*)&(buffer[248+8])); // (62-1)*4 ligne*nb bytes + 12 offset horizon 4
    u_int16_t speed_bytes = (uint16_t)(speed*3.6);
    
    // split into 5 and 5 bits so that none of the high and low part can go to 255 and thus be used as end marquer
    u_int8_t speed_bytes_h = (speed_bytes>>5)  & 0b00011111; // 5 upper bits
    u_int8_t speed_bytes_l = speed_bytes & 0b00011111; // 5 lower bits
    
    float rpm = *((float*)&(buffer[16])); 
    u_int8_t rpm_byte = rpm/100;
    
    serialbuffer[1] = speed_bytes_h;
    serialbuffer[2] = speed_bytes_l;
    serialbuffer[3] = rpm_byte;
    write(serial_port,&serialbuffer,sizeof(serialbuffer));

  }
  
  close(serial_port);
  close(listenfd);
  return 0;
}