package cl.rvillablanca;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        System.out.println("Hello from Java");
        if (args.length > 0) {
            System.out.println("Arguments length:" + args.length);
            for (String arg : args) {
                System.out.println(arg);
            }
        }
        System.out.println("Waiting");
        Thread.sleep(1000 * 10);
        System.out.println("Done");
    }
}