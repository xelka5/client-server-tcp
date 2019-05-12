enum operation{Order, Refill, GetAccumulation, GetClub};
enum status{Ack, Nack};

typedef struct Article {
   int id;
   char name[50];
   double price;
   int quantity;
}Article;  

typedef struct Package {
   enum operation packageType;
   char clubName[50];
   struct Article article;
   char date[10];
}Package;

typedef struct Response {
   enum status responseStatus;
   char responsePayload[200];
}Response;


