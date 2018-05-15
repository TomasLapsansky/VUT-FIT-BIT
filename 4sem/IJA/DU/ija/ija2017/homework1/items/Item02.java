package ija.ija2017.homework1.items;

public class Item02 extends AbstractItem {

    public Item02(String name, int port) {
        super(name, port);
    }

    @Override
    public void execute() {
        super.state += 2;
    }
}
