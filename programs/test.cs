namespace HelloWorld{
    class Hello{
        public static void Main(string[] args){
            int a = 10;
            int b = 1;
            double jj = 10.0;
            string[] c = new string[10];
            c[0] = "haha";
            string d = "hi_lupita23 fdss";
            string dd = "jeje";
            string empty_string = "";
            char e = 'e';
            bool f = true;
            bool eresRaku = false;
            if(eresRaku) a = 1;
            if( a <b){ // Esto deberia fallar!
                a = 10;
            }else b = a;
            if(d < dd) d = "ok";
            // if(d){ // Esto deberia fallar!
            //     int count = 10;
            // }else if(true){

            // }
            // while(d){
                
            // }
            for(int i=0; i<b; i++){
                b++;
            }
            var h = a % b;
            double j = jj;
            while(b > 0){
                b--;
            }

            do{
                a = b;
                b++;
            }while(a < 20);
        }
    }
}