package cl.rvillablanca;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello from Java");
        if (args.length > 0) {
            System.out.println("Arguments length:" + args.length);
            for (String arg : args) {
                System.out.println(arg);
            }
        }
    }
}